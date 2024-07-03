#ifndef REGISTERED_KEYBOARD_COMMAND_H
#define REGISTERED_KEYBOARD_COMMAND_H

#include <roingine/input.h>

namespace roingine {
	class Command;

	class RegisteredKeyboardCommand final {
	public:
		RegisteredKeyboardCommand(InputKeys inputKey, KeyEventType eventType, std::unique_ptr<Command> pCommand);

		~RegisteredKeyboardCommand();

		RegisteredKeyboardCommand(RegisteredKeyboardCommand &&) noexcept;

		RegisteredKeyboardCommand(RegisteredKeyboardCommand const &) = delete;

		RegisteredKeyboardCommand &operator=(RegisteredKeyboardCommand &&) noexcept;

		RegisteredKeyboardCommand &operator=(RegisteredKeyboardCommand const &) = delete;

	private:
		Command     *m_rpCommand;
		InputKeys    m_InputKey;
		KeyEventType m_EventType;
	};
}// namespace roingine

#endif// REGISTERED_KEYBOARD_COMMAND_H
