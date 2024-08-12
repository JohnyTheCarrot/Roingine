#include "player.h"

#include "../living_entity_command.h"
#include "bomberman.h"
#include "level_flyweight.h"
#include "living_entity.h"

#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>
#include <roingine/roingine.h>

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

	float const Player::c_WalkSoundDistance{50.f};
	float const Player::c_Size{LevelFlyweight::c_TileSize - 20};
	float const Player::c_WalkSpeed{200.f};

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
		bool isMoving{false};
		if (IsKbOrControllerInputDown(c_UpKbButton, c_UpControllerButton, true)) {
			m_pMoveUpCommand->Execute();
			isMoving = true;
		}

		if (IsKbOrControllerInputDown(c_DownKbButton, c_DownControllerButton, true)) {
			m_pMoveDownCommand->Execute();
			isMoving = true;
		}

		if (IsKbOrControllerInputDown(c_LeftKbButton, c_LeftControllerButton, true)) {
			m_pMoveLeftCommand->Execute();
			isMoving = true;
		}

		if (IsKbOrControllerInputDown(c_RightKbButton, c_RightControllerButton, true)) {
			m_pMoveRightCommand->Execute();
			isMoving = true;
		}

		if (!isMoving)
			m_pIdleCommand->Execute();

		if (IsKbOrControllerInputDown(c_BombKbButton, c_BombControllerButton, false)) {
			m_pPrimaryActionCommand->Execute();
		}
	}

	Player::Player(
	        roingine::GameObject gameObject, roingine::Transform &cameraTransform, LevelFlyweight const &,
	        bool keyboardSupported
	)
	    : Component{gameObject}// , m_hExplosionHandler{event_queue::EventQueue::GetInstance()
	    //                               .AttachEventHandler<event_queue::EventType::Explosion>(
	    //                                       [this](event_queue::ExplosionData const &data) {
	    //                                        auto const &pos{
	    //                                                m_rpTransform->GetWorldPosition() + glm::vec2{c_Size, c_Size}
	    //                                        };
	    //
	    //                                        if (data.IsInExplosion(pos)) {
	    //                                         std::cout << "dieded" << std::endl;
	    //                                        }
	    //                                       }
	    //                               )}
	    , m_rpLivingEntityComponent{&gameObject.GetComponent<LivingEntity>()}
	    , m_pMoveUpCommand{std::make_unique<LivingEntityCommand>(*m_rpLivingEntityComponent, MoveInstruction::Up)}
	    , m_pMoveDownCommand{std::make_unique<LivingEntityCommand>(*m_rpLivingEntityComponent, MoveInstruction::Down)}
	    , m_pMoveLeftCommand{std::make_unique<LivingEntityCommand>(*m_rpLivingEntityComponent, MoveInstruction::Left)}
	    , m_pMoveRightCommand{std::make_unique<LivingEntityCommand>(*m_rpLivingEntityComponent, MoveInstruction::Right)}
	    , m_pIdleCommand{std::make_unique<LivingEntityCommand>(*m_rpLivingEntityComponent, MoveInstruction::Idle)}
	    , m_pPrimaryActionCommand{std::make_unique<LivingEntityCommand>(*m_rpLivingEntityComponent, Action::Primary)}
	    , m_pSeconaryActionCommand{std::make_unique<LivingEntityCommand>(*m_rpLivingEntityComponent, Action::Secondary)}
	    , m_rpCameraTransform{&cameraTransform}
	    , m_rpTransform{&gameObject.GetComponent<roingine::Transform>()}
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
	}
}// namespace bomberman
