#include <roingine/commands/registered_controller_command.h>
#include <roingine/controller.h>

namespace roingine {
	RegisteredControllerCommand::RegisteredControllerCommand(
	        Controller *rpController, ControllerButton button, ButtonState buttonState,
	        std::unique_ptr<Command> pCommand
	)
	    : m_rpController{rpController}
	    , m_rpCommand{pCommand.get()}
	    , m_Button{button}
	    , m_ButtonState{buttonState} {
		m_rpController->RegisterCommand(button, buttonState, std::move(pCommand));
	}

	RegisteredControllerCommand::~RegisteredControllerCommand() {
		if (m_rpCommand == nullptr)
			return;

		m_rpController->RemoveCommand(m_Button, m_ButtonState, m_rpCommand);
	}

	RegisteredControllerCommand::RegisteredControllerCommand(RegisteredControllerCommand &&other) noexcept
	    : m_rpController{other.m_rpController}
	    , m_rpCommand{other.m_rpCommand}
	    , m_Button{other.m_Button}
	    , m_ButtonState{other.m_ButtonState} {
		other.m_rpCommand = nullptr;
	}

	RegisteredControllerCommand &RegisteredControllerCommand::operator=(RegisteredControllerCommand &&other) noexcept {
		if (this == &other)
			return *this;

		m_rpController    = other.m_rpController;
		m_rpCommand       = other.m_rpCommand;
		m_Button          = other.m_Button;
		m_ButtonState     = other.m_ButtonState;
		other.m_rpCommand = nullptr;

		return *this;
	}
}// namespace roingine
