#include "bomberman.h"

#include "../audio.h"
#include "moving_entity.h"
#include "player.h"

#include <roingine/components/animation_renderer.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>
#include <roingine/roingine.h>

namespace bomberman {
	float const Bomberman::c_Size{70.f};
	float const Bomberman::c_Speed{200.f};
	float const Bomberman::c_WalkSoundDistance{50.f};
	float const Bomberman::c_InvincibilityAfterHurtSec{5.f};

	constexpr glm::vec2 c_UpVec{0, 1};
	constexpr glm::vec2 c_DownVec{0, -1};
	constexpr glm::vec2 c_LeftVec{-1, 0};
	constexpr glm::vec2 c_RightVec{1, 0};

	struct AnimationFrameRange final {
		int start{};
		int end{};
	};

	constexpr AnimationFrameRange c_AnimWalkLeft{0, 3};
	constexpr AnimationFrameRange c_AnimWalkDown{3, 6};
	constexpr AnimationFrameRange c_AnimWalkRight{6, 9};
	constexpr AnimationFrameRange c_AnimWalkUp{9, 11};

	Bomberman &BombermanFSMNode::GetBomberman() const {
		return *m_rpBomberman;
	}

	BombermanFSMNode::BombermanFSMNode(Bomberman &bomberman)
	    : m_rpBomberman{&bomberman} {
	}

	void BombermanIdle::OnEnter() {
		auto &animRenderer{GetBomberman().GetAnimRenderer()};
		animRenderer.SetPaused(true);
	}

	void BombermanIdle::OnExit() {
		auto &animRenderer{GetBomberman().GetAnimRenderer()};
		animRenderer.SetPaused(false);
	}

	std::unique_ptr<FiniteStateMachine<LivingEntityInstruction>>
	BombermanIdle::Update(LivingEntityInstruction const &input) {
		if (std::holds_alternative<DieInstruction>(input))
			return std::make_unique<BombermanDying>(GetBomberman());

		if (std::holds_alternative<Action>(input)) {
			if (auto const action{std::get<Action>(input)}; action == Action::Primary)
				GetBomberman().PlaceBomb();

			return nullptr;
		}

		if (auto const movementInstruction{std::get<MoveInstruction>(input)};
		    movementInstruction != MoveInstruction::Idle)
			return std::make_unique<BombermanWalking>(GetBomberman());

		return nullptr;
	}

	std::unique_ptr<FiniteStateMachine<LivingEntityInstruction>>
	BombermanWalking::Update(LivingEntityInstruction const &input) {
		if (std::holds_alternative<DieInstruction>(input))
			return std::make_unique<BombermanDying>(GetBomberman());

		if (std::holds_alternative<Action>(input)) {
			if (auto const action{std::get<Action>(input)}; action == Action::Primary)
				GetBomberman().PlaceBomb();

			return nullptr;
		}

		auto      &bomberman{GetBomberman()};
		auto const movementInstruction{std::get<MoveInstruction>(input)};

		if (movementInstruction == MoveInstruction::Idle)
			return std::make_unique<BombermanIdle>(bomberman);

		auto &animRenderer{bomberman.GetAnimRenderer()};

		switch (movementInstruction) {
			case MoveInstruction::Up:
				bomberman.Move(c_UpVec);
				animRenderer.SetFrameRange(c_AnimWalkUp.start, c_AnimWalkUp.end);
				break;
			case MoveInstruction::Down:
				bomberman.Move(c_DownVec);
				animRenderer.SetFrameRange(c_AnimWalkDown.start, c_AnimWalkDown.end);
				break;
			case MoveInstruction::Left:
				bomberman.Move(c_LeftVec);
				animRenderer.SetFrameRange(c_AnimWalkLeft.start, c_AnimWalkLeft.end);
				break;
			case MoveInstruction::Right:
				bomberman.Move(c_RightVec);
				animRenderer.SetFrameRange(c_AnimWalkRight.start, c_AnimWalkRight.end);
				break;
			default:
				break;
		}

		return nullptr;
	}

	void BombermanDying::OnEnter() {
		GetBomberman().Die();
	}

	std::unique_ptr<FiniteStateMachine<LivingEntityInstruction>>
	BombermanDying::Update(std::variant<MoveInstruction, DieInstruction, Action> const &) {
		return nullptr;
	}

	Bomberman::Bomberman(roingine::GameObject gameObject, LevelFlyweight const &levelFlyweight)
	    : Component{gameObject}
	    , m_rpRectCollider{&gameObject.AddComponent<roingine::RectCollider>(c_Size, c_Size)}
	    , m_rpMovingEntityComponent{&gameObject.AddComponent<MovingEntity>(levelFlyweight, c_Speed)}
	    , m_rpAnimRenderer{&gameObject.AddComponent<roingine::AnimationRenderer>(
	              roingine::AnimationRenderer::AnimationInfo{
	                      .fileName        = "res/img/player.png",
	                      .numFrames       = 19,
	                      .secondsPerFrame = 0.1f
	              },
	              c_Size, c_Size, roingine::ScalingMethod::NearestNeighbor
	      )}
	    , m_rpLivingEntityComponent{&gameObject.AddComponent<LivingEntity>(std::make_unique<BombermanIdle>(*this))}
	    , m_rpTransform{&GetGameObject().GetComponent<roingine::Transform>()}
	    , m_pPlaceBombCommand{std::make_unique<PlaceBombCommand>(GetGameObject())} {
	}

	roingine::AnimationRenderer &Bomberman::GetAnimRenderer() const {
		return *m_rpAnimRenderer;
	}

	roingine::Transform &Bomberman::GetTransform() const {
		return *m_rpTransform;
	}

	void Bomberman::Move(glm::vec2 const &direction) const {
		m_rpMovingEntityComponent->Move(direction);
	}

	void Bomberman::PlaceBomb() const {
		auto *rpPlayer{GetGameObject().GetOptionalComponent<Player>()};
		if (rpPlayer == nullptr)
			return;

		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::BombPlaceRequest>(
		        &rpPlayer->GetPlayerInfo(), GetTransform().GetWorldPosition()
		);
	}

	void Bomberman::Die() const {
		m_rpAnimRenderer->SetFrameRange(11, 18);
	}

	bool Bomberman::IsHurting() const {
		return m_rpLivingEntityComponent->GetInvulnerable();
	}

	void Bomberman::Update() {
		if (auto const position{m_rpTransform->GetWorldPosition()};
		    distance(m_PreviousWalkSoundPosition, position) > c_WalkSoundDistance) {

			auto const xDistance{std::abs(m_PreviousWalkSoundPosition.x - position.x)};
			auto const yDistance{std::abs(m_PreviousWalkSoundPosition.y - position.y)};
			m_PreviousWalkSoundPosition = position;

			auto const sound{xDistance > yDistance ? audio::Sound::WalkHorizontal : audio::Sound::WalkVertical};

			audio::AudioServiceLocator::GetService().Play(sound);
		}
	}
}// namespace bomberman
