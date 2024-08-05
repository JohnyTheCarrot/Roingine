#include "player.h"

#include "../move_command.h"
#include "../place_bomb_command.h"
#include "level_flyweight.h"
#include "moving_entity.h"

#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>

namespace bomberman {
	constexpr glm::vec2 UP{0, 1};
	constexpr glm::vec2 DOWN{0, -1};
	constexpr glm::vec2 LEFT{-1, 0};
	constexpr glm::vec2 RIGHT{1, 0};

	PlayerKeyboardCommands::PlayerKeyboardCommands(MovingEntity *rpMovingEntityComponent)
	    : m_KeyboardUpCommand{roingine::InputKeys::W, roingine::KeyEventType::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, UP)}
	    , m_KeyboardDownCommand{roingine::InputKeys::S, roingine::KeyEventType::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, DOWN)}
	    , m_KeyboardLeftCommand{roingine::InputKeys::A, roingine::KeyEventType::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, LEFT)}
	    , m_KeyboardRightCommand{roingine::InputKeys::D, roingine::KeyEventType::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, RIGHT)}
	    , m_PlaceBombCommand{
	              roingine::InputKeys::E, roingine::KeyEventType::Down,
	              std::make_unique<PlaceBombCommand>(
	                      rpMovingEntityComponent->GetGameObject().GetComponent<roingine::Transform>()
	              )
	      } {
	}

	ControllerCommands::ControllerCommands(roingine::Controller *rpController, MovingEntity *rpMovingEntityComponent)
	    : m_ControllerUpCommand{rpController, roingine::ControllerButton::DPadUp, roingine::ButtonState::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, UP)}
	    , m_ControllerDownCommand{rpController, roingine::ControllerButton::DPadDown, roingine::ButtonState::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, DOWN)}
	    , m_ControllerLeftCommand{rpController, roingine::ControllerButton::DPadLeft, roingine::ButtonState::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, LEFT)}
	    , m_ControllerRightCommand{rpController, roingine::ControllerButton::DPadRight, roingine::ButtonState::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, RIGHT)}
	    , m_rpController{rpController} {
	}

	Player::Player(
	        roingine::GameObject &gameObject, roingine::Transform &cameraTransform,
	        MovingEntity *rpMovingEntityComponent, bool isPlayer1
	)
	    : Component{gameObject}
	    , m_KeyboardCommands{isPlayer1 ? std::make_optional(rpMovingEntityComponent) : std::nullopt}
	    , m_rpTransform{&gameObject.GetComponent<roingine::Transform>()}
	    , m_rpCameraTransform{&cameraTransform}
	    , m_rpMovingEntityComponent{rpMovingEntityComponent}
	    , m_rpRectCollider{&gameObject.GetComponent<roingine::RectCollider>()} {
	}

	void Player::TieToController(roingine::Controller *rpController) {
		m_ControllerCommands.emplace(rpController, m_rpMovingEntityComponent);
	}

	void Player::DisconnectController() {
		m_ControllerCommands.reset();
	}

	bool Player::HasSpecificController(roingine::Controller const *rpController) const {
		return m_ControllerCommands.has_value() && m_ControllerCommands->m_rpController == rpController;
	}

	bool Player::HasConnectedController() const {
		return m_ControllerCommands.has_value();
	}

	void Player::Update() {
		m_rpCameraTransform->SetLocalPosition(m_rpTransform->GetWorldPosition());
	}
}// namespace bomberman
