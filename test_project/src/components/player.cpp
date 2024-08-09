#include "player.h"

#include "../audio.h"
#include "../move_command.h"
#include "../place_bomb_command.h"
#include "level_flyweight.h"
#include "moving_entity.h"
#include "roingine/roingine.h"

#include <roingine/components/animation_renderer.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>

namespace bomberman {
	constexpr glm::vec2 c_UpVec{0, 1};
	constexpr glm::vec2 c_DownVec{0, -1};
	constexpr glm::vec2 c_LeftVec{-1, 0};
	constexpr glm::vec2 c_RightVec{1, 0};

	constexpr auto c_UpKbButton{roingine::InputKeys::W};
	constexpr auto c_LeftKbButton{roingine::InputKeys::A};
	constexpr auto c_DownKbButton{roingine::InputKeys::S};
	constexpr auto c_RightKbButton{roingine::InputKeys::D};
	constexpr auto c_BombKbButton{roingine::InputKeys::E};

	constexpr auto c_LeftControllerButton{roingine::ControllerButton::DPadLeft};
	constexpr auto c_RightControllerButton{roingine::ControllerButton::DPadRight};
	constexpr auto c_UpControllerButton{roingine::ControllerButton::DPadUp};
	constexpr auto c_DownControllerButton{roingine::ControllerButton::DPadDown};
	constexpr auto c_BombControllerButton{roingine::ControllerButton::X};

	struct AnimationFrameRange final {
		int start{};
		int end{};
	};

	constexpr AnimationFrameRange c_AnimWalkLeft{0, 3};
	constexpr AnimationFrameRange c_AnimWalkDown{3, 6};
	constexpr AnimationFrameRange c_AnimWalkRight{6, 9};
	constexpr AnimationFrameRange c_AnimWalkUp{9, 11};

	float const Player::WALK_SOUND_DISTANCE{50.f};
	float const Player::SIZE{LevelFlyweight::c_TileSize - 20};

	bool Player::IsKbOrControllerInputDown(roingine::InputKeys key, roingine::ControllerButton button, bool checkIfHeld)
	        const {
		if (auto const state{roingine::KeyboardInput::GetService().GetKeyState(key)};
		    m_HasKeyboardSupport &&
		    (checkIfHeld ? state == roingine::KeyEventType::Held || state == roingine::KeyEventType::LongPress
		                 : state == roingine::KeyEventType::Down))
			return true;

		if (m_rpController == nullptr)
			return false;

		auto const expectedState{checkIfHeld ? roingine::ButtonState::Held : roingine::ButtonState::Pressed};
		return m_rpController->GetButtonState(button) == expectedState;
	}

	void Player::HandleInput() const {
		// I realise I *could* use an FSM here, but doing so would be overkill for this simple case, in my opinion.
		// The FSM would be more useful if the player had more states, such as "placing bomb", etc.
		// Instead every node in the FSM does practically the exact same, with the only difference the current node cannot be transitioned to.
		// With this in mind, I decided to keep it simple.
		bool didMove{false};

		if (IsKbOrControllerInputDown(c_UpKbButton, c_UpControllerButton, true)) {
			m_rpMovingEntityComponent->Move(c_UpVec);
			m_rpAnimRenderer->SetFrameRange(c_AnimWalkUp.start, c_AnimWalkUp.end);
			didMove = true;
		}

		if (IsKbOrControllerInputDown(c_DownKbButton, c_DownControllerButton, true)) {
			m_rpMovingEntityComponent->Move(c_DownVec);
			m_rpAnimRenderer->SetFrameRange(c_AnimWalkDown.start, c_AnimWalkDown.end);
			didMove = true;
		}

		if (IsKbOrControllerInputDown(c_LeftKbButton, c_LeftControllerButton, true)) {
			m_rpMovingEntityComponent->Move(c_LeftVec);
			m_rpAnimRenderer->SetFrameRange(c_AnimWalkLeft.start, c_AnimWalkLeft.end);
			didMove = true;
		}

		if (IsKbOrControllerInputDown(c_RightKbButton, c_RightControllerButton, true)) {
			m_rpMovingEntityComponent->Move(c_RightVec);
			m_rpAnimRenderer->SetFrameRange(c_AnimWalkRight.start, c_AnimWalkRight.end);
			didMove = true;
		}

		m_rpAnimRenderer->SetPaused(!didMove);

		if (IsKbOrControllerInputDown(c_BombKbButton, c_BombControllerButton, false)) {
			m_pPlaceBombCommand->Execute();
		}
	}

	Player::Player(
	        roingine::GameObject &gameObject, roingine::Transform &cameraTransform,
	        LevelFlyweight const &levelFlyweight, bool keyboardSupported
	)
	    : Component{gameObject}
	    , m_rpRectCollider{&gameObject.AddComponent<roingine::RectCollider>(SIZE, SIZE)}
	    , m_rpMovingEntityComponent{&gameObject.AddComponent<MovingEntity>(levelFlyweight, 200.f)}
	    , m_rpMoveUpCommand{std::make_unique<MoveCommand>(m_rpMovingEntityComponent, c_UpVec)}
	    , m_rpMoveDownCommand{std::make_unique<MoveCommand>(m_rpMovingEntityComponent, c_DownVec)}
	    , m_rpMoveLeftCommand{std::make_unique<MoveCommand>(m_rpMovingEntityComponent, c_LeftVec)}
	    , m_rpMoveRightCommand{std::make_unique<MoveCommand>(m_rpMovingEntityComponent, c_RightVec)}
	    , m_pPlaceBombCommand{std::make_unique<PlaceBombCommand>(GetGameObject().GetComponent<roingine::Transform>())}
	    , m_rpTransform{&gameObject.GetComponent<roingine::Transform>()}
	    , m_rpCameraTransform{&cameraTransform}
	    , m_rpAnimRenderer{&gameObject.AddComponent<roingine::AnimationRenderer>(
	              "res/img/player.png", 19, 0.1f, SIZE, SIZE, roingine::ScalingMethod::NearestNeighbor
	      )}
	    , m_HasKeyboardSupport{keyboardSupported} {
	}

	void Player::TieToController(roingine::Controller *rpController) {
		m_rpController = rpController;
	}

	void Player::DisconnectController() {
		m_rpController = nullptr;
	}

	bool Player::HasSpecificController(roingine::Controller const *rpController) const {
		return m_rpController == rpController;
	}

	bool Player::HasConnectedController() const {
		return m_rpController != nullptr;
	}

	void Player::Update() {
		m_rpCameraTransform->SetLocalPosition(m_rpTransform->GetWorldPosition());

		HandleInput();

		if (auto const position{m_rpTransform->GetWorldPosition()};
		    distance(m_PreviousWalkSoundPosition, position) > WALK_SOUND_DISTANCE) {

			auto const xDistance{std::abs(m_PreviousWalkSoundPosition.x - position.x)};
			auto const yDistance{std::abs(m_PreviousWalkSoundPosition.y - position.y)};
			m_PreviousWalkSoundPosition = position;

			auto const sound{xDistance > yDistance ? audio::Sound::WalkHorizontal : audio::Sound::WalkVertical};

			audio::AudioServiceLocator::GetService().Play(sound);
		}
	}
}// namespace bomberman
