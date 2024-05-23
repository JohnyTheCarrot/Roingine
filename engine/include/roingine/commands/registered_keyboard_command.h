#ifndef REGISTERED_KEYBOARD_COMMAND_H
#define REGISTERED_KEYBOARD_COMMAND_H

#include <roingine/input.h>

namespace roingine {
	class Command;

	class RegisteredKeyboardCommand final {
	public:
		RegisteredKeyboardCommand(InputKeys inputKey, KeyEventType eventType, std::unique_ptr<Command> pCommand);

		~RegisteredKeyboardCommand();

		RegisteredKeyboardCommand(RegisteredKeyboardCommand &&);

		RegisteredKeyboardCommand(RegisteredKeyboardCommand const &) = delete;

		RegisteredKeyboardCommand &operator=(RegisteredKeyboardCommand &&);

		RegisteredKeyboardCommand &operator=(RegisteredKeyboardCommand const &) = delete;

	private:
		InputKeys    m_InputKey;
		KeyEventType m_EventType;
		Command     *m_rpCommand;
	};
}// namespace roingine

#endif// REGISTERED_KEYBOARD_COMMAND_H
