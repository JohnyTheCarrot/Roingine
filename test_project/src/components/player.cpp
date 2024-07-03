#include "player.h"

#include "../move_command.h"

namespace bomberman {
	constexpr glm::vec2 UP{0, 1};
	constexpr glm::vec2 DOWN{0, -1};
	constexpr glm::vec2 LEFT{-1, 0};
	constexpr glm::vec2 RIGHT{1, 0};

	PlayerKeyboardCommands::PlayerKeyboardCommands(MovingEntity *rpMovingEntityComponent)
	    : m_KeyboardUpCommand{roingine::InputKeys::W, roingine::KeyEventType::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, UP)}
	    , m_KeyboardDownCommand{roingine::InputKeys::S, roingine::KeyEventType::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, DOWN)}
	    , m_KeyboardLeftCommand{roingine::InputKeys::A, roingine::KeyEventType::Held, std::make_unique<MoveCommand>(rpMovingEntityComponent, LEFT)}
	    , m_KeyboardRightCommand{
	              roingine::InputKeys::D, roingine::KeyEventType::Held,
	              std::make_unique<MoveCommand>(rpMovingEntityComponent, RIGHT)
	      } {
	}

	Player::Player(roingine::GameObject const &gameObject, MovingEntity *rpMovingEntityComponent, bool isPlayer1)
	    : roingine::Component{gameObject}
	    , m_KeyboardCommands{isPlayer1 ? std::make_optional(rpMovingEntityComponent) : std::nullopt} {
	}
}// namespace bomberman
