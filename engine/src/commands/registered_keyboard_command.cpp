#include <roingine/commands/command.h>
#include <roingine/commands/registered_keyboard_command.h>
#include <roingine/roingine.h>

namespace roingine {
	RegisteredKeyboardCommand::RegisteredKeyboardCommand(
	        InputKeys inputKey, KeyEventType eventType, std::unique_ptr<Command> pCommand
	)
	    : m_rpCommand{pCommand.get()}
	    , m_InputKey{inputKey}
	    , m_EventType{eventType} {
		KeyboardInput::GetService().AddCommand(inputKey, eventType, std::move(pCommand));
	}

	RegisteredKeyboardCommand::~RegisteredKeyboardCommand() {
		if (m_rpCommand == nullptr)
			return;

		KeyboardInput::GetService().RemoveCommand(m_InputKey, m_EventType, m_rpCommand);
	}

	RegisteredKeyboardCommand::RegisteredKeyboardCommand(RegisteredKeyboardCommand &&other) noexcept
	    : m_rpCommand{other.m_rpCommand}
	    , m_InputKey{other.m_InputKey}
	    , m_EventType{other.m_EventType} {
		other.m_rpCommand = nullptr;
	}

	RegisteredKeyboardCommand &RegisteredKeyboardCommand::operator=(RegisteredKeyboardCommand &&other) noexcept {
		if (this == &other)
			return *this;

		m_InputKey        = other.m_InputKey;
		m_EventType       = other.m_EventType;
		m_rpCommand       = other.m_rpCommand;
		other.m_rpCommand = nullptr;

		return *this;
	}
}// namespace roingine
