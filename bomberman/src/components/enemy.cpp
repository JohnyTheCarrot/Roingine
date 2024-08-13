#include "enemy.h"

#include "enemy_ai.h"
#include "level_flyweight.h"
#include "moving_entity.h"
#include "temporary_object.h"

#include <glm/common.hpp>
#include <roingine/components/animation_renderer.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>
#include <roingine/scene.h>

namespace bomberman {
	constexpr glm::vec2 c_UpVec{0, 1};
	constexpr glm::vec2 c_DownVec{0, -1};
	constexpr glm::vec2 c_LeftVec{-1, 0};
	constexpr glm::vec2 c_RightVec{1, 0};

	Enemy &EnemyFSMNode::GetEnemy() const {
		return *m_rpEnemy;
	}

	EnemyFSMNode::EnemyFSMNode(Enemy &enemy)
	    : m_rpEnemy{&enemy} {
	}

	void EnemyIdle::OnEnter() {
		GetEnemy().SetAnimationPaused(true);
	}

	void EnemyIdle::OnExit() {
		GetEnemy().SetAnimationPaused(false);
	}

	std::unique_ptr<FiniteStateMachine<LivingEntityInstruction>>
	EnemyIdle::Update(std::variant<MoveInstruction, DieInstruction, Action> const &input) {
		if (std::holds_alternative<Action>(input))
			return nullptr;

		if (std::holds_alternative<DieInstruction>(input))
			return std::make_unique<NullFSM<LivingEntityInstruction>>();

		switch (std::get<MoveInstruction>(input)) {
			case MoveInstruction::Up:
			case MoveInstruction::Down:
			case MoveInstruction::Left:
			case MoveInstruction::Right:
				return std::make_unique<EnemyWalking>(GetEnemy());
			case MoveInstruction::Idle:
				return nullptr;
		}

		// should never reach here
		assert(false);
		return nullptr;
	}

	std::unique_ptr<FiniteStateMachine<std::variant<MoveInstruction, DieInstruction, Action>>>
	EnemyWalking::Update(std::variant<MoveInstruction, DieInstruction, Action> const &input) {
		if (std::holds_alternative<Action>(input))
			return nullptr;

		if (std::holds_alternative<DieInstruction>(input))
			return std::make_unique<NullFSM<LivingEntityInstruction>>();

		switch (std::get<MoveInstruction>(input)) {
			case MoveInstruction::Up:
				GetEnemy().Move(c_UpVec);
				break;
			case MoveInstruction::Down:
				GetEnemy().Move(c_DownVec);
				break;
			case MoveInstruction::Left:
				GetEnemy().Move(c_LeftVec);
				break;
			case MoveInstruction::Right:
				GetEnemy().Move(c_RightVec);
				break;
			case MoveInstruction::Idle:
				return std::make_unique<EnemyIdle>(GetEnemy());
		}

		return nullptr;
	}

	glm::vec2 Enemy::DirectionToVec(Direction direction) {
		switch (direction) {
			case Direction::Up:
				return {0, 1};
			case Direction::Down:
				return {0, -1};
			case Direction::Left:
				return {-1, 0};
			default:
				return {1, 0};
		}
	}

	Enemy::Enemy(roingine::GameObject gameObject, LevelFlyweight const &levelFlyweight, EnemyInfo const &info)
	    : Component{gameObject}
	    , m_rpRectCollider{&gameObject.AddComponent<roingine::RectCollider>(c_Size, c_Size)}
	    , m_rpMovingEntity{&gameObject.AddComponent<MovingEntity>(
	              levelFlyweight,
	              [&info] {
		              switch (info.speed) {
			              default:
				              return 150.f;
			              case Speed::Normal:
				              return 200.f;
			              case Speed::Fast:
				              return 250.f;
		              }
	              }()
	      )}
	    , m_rpAnimRenderer{&gameObject.AddComponent<roingine::AnimationRenderer>(
	              info.walkAnimInfo, c_Size, c_Size, roingine::ScalingMethod::NearestNeighbor
	      )}
	    , m_rpLivingEntity{&gameObject.AddComponent<LivingEntity>(std::make_unique<EnemyIdle>(*this))}
	    , deathAnimInfo{info.deathAnimInfo}
	    , m_ScoreOnKill{info.scoreOnKill} {
	}

	float const Enemy::c_Size{70.f};

	bool Enemy::Move(glm::vec2 direction) const {
		return m_rpMovingEntity->Move(direction);
	}

	void Enemy::FlipDirection() const {
		m_rpAnimRenderer->SetFlipped(!m_rpAnimRenderer->GetFlipped());
	}

	bool Enemy::IsAnimationPaused() const {
		return m_rpAnimRenderer->IsPaused();
	}

	void Enemy::SetAnimationPaused(bool paused) const {
		m_rpAnimRenderer->SetPaused(paused);
	}

	roingine::AnimationRenderer &Enemy::GetAnimRenderer() const {
		return *m_rpAnimRenderer;
	}

	int Enemy::GetScoreOnKill() const {
		return m_ScoreOnKill;
	}

	void Enemy::Kill() {
		m_rpLivingEntity->Instruct(DieInstruction{});

		auto enemyGameObject{GetGameObject()};
		enemyGameObject.DestroyComponent<roingine::AnimationRenderer>();

		auto const &animRenderer{enemyGameObject.AddComponent<roingine::AnimationRenderer>(
		        deathAnimInfo, c_Size, c_Size, roingine::ScalingMethod::NearestNeighbor
		)};
		enemyGameObject.AddComponent<TemporaryObject>(animRenderer.GetAnimationRangeDuration());

		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::EnemyDied>(GetScoreOnKill());
	}

	void Enemy::SpawnEnemy(
	        roingine::Scene &scene, LevelFlyweight const &levelFlyweight, EnemyInfo const &info, glm::vec2 position
	) {
		auto enemyObject{scene.AddGameObject()};
		enemyObject.AddComponent<roingine::Transform>(position, 0.f);
		enemyObject.AddComponent<roingine::RectCollider>(c_Size, c_Size);
		enemyObject.AddComponent<Enemy>(levelFlyweight, info);
		enemyObject.AddComponent<EnemyAi>(EnemyAi::AIType::Simple);
	}

	Enemy::FreeDirections Enemy::GetFreeDirections() const {
		auto const &levelFlyweight{m_rpMovingEntity->GetLevelFlyweight()};
		auto const &transform{GetGameObject().GetComponent<roingine::Transform>()};
		auto const  center{transform.GetWorldPosition() + glm::vec2{c_Size / 2, c_Size / 2}};
		auto const [posX, posY]{levelFlyweight.PositionToGrid(center)};

		auto const isFree{[&](int offsetX, int offsetY) {
			return levelFlyweight.GetTileType(static_cast<int>(posX) + offsetX, static_cast<int>(posY) + offsetY) ==
			       LevelFlyweight::TileType::Nothing;
		}};

		return {.up = isFree(0, 1), .down = isFree(0, -1), .left = isFree(-1, 0), .right = isFree(1, 0)};
	}

	bool Enemy::IsInOneTile() const {
		auto const &levelFlyweight{m_rpMovingEntity->GetLevelFlyweight()};
		auto const &transform{GetGameObject().GetComponent<roingine::Transform>()};
		auto const  pos{transform.GetWorldPosition()};
		auto const  topLeft{pos};
		auto const  topRight{pos + glm::vec2{c_Size, 0}};
		auto const  bottomLeft{pos + glm::vec2{0, c_Size}};
		auto const  bottomRight{pos + glm::vec2{c_Size, c_Size}};

		auto const topLeftGrid{levelFlyweight.PositionToGrid(topLeft)};
		auto const topRightGrid{levelFlyweight.PositionToGrid(topRight)};
		auto const bottomLeftGrid{levelFlyweight.PositionToGrid(bottomLeft)};
		auto const bottomRightGrid{levelFlyweight.PositionToGrid(bottomRight)};

		return topLeftGrid == topRightGrid && topLeftGrid == bottomLeftGrid && topLeftGrid == bottomRightGrid;
	}
}// namespace bomberman
