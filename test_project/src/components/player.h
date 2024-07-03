#ifndef PLAYER_H
#define PLAYER_H

#include <roingine/commands/registered_keyboard_command.h>
#include <roingine/components/component.h>

namespace bomberman {
	class MovingEntity;

	class PlayerKeyboardCommands final {
		roingine::RegisteredKeyboardCommand m_KeyboardUpCommand;
		roingine::RegisteredKeyboardCommand m_KeyboardDownCommand;
		roingine::RegisteredKeyboardCommand m_KeyboardLeftCommand;
		roingine::RegisteredKeyboardCommand m_KeyboardRightCommand;

	public:
		explicit PlayerKeyboardCommands(MovingEntity *rpMovingEntityComponent);
	};

	class Player final : public roingine::Component {
		std::optional<PlayerKeyboardCommands> m_KeyboardCommands;

	public:
		Player(roingine::GameObject const &gameObject, MovingEntity *rpMovingEntityComponent, bool isPlayer1);
	};
}// namespace bomberman

#endif//PLAYER_H
