#ifndef REGISTERED_CONTROLLER_COMMAND_H
#define REGISTERED_CONTROLLER_COMMAND_H
#include "roingine/controller.h"

namespace roingine {
	class Controller;

	class RegisteredControllerCommand final {
		Controller      *m_rpController;
		Command         *m_rpCommand;
		ControllerButton m_Button;
		ButtonState      m_ButtonState;

	public:
		RegisteredControllerCommand(
		        Controller *rpController, ControllerButton button, ButtonState buttonState,
		        std::unique_ptr<Command> pCommand
		);

		~RegisteredControllerCommand();

		RegisteredControllerCommand(RegisteredControllerCommand &&) noexcept;

		RegisteredControllerCommand &operator=(RegisteredControllerCommand &&) noexcept;

		RegisteredControllerCommand(RegisteredControllerCommand const &) = delete;

		RegisteredControllerCommand &operator=(RegisteredControllerCommand const &) = delete;
	};
}// namespace roingine

#endif//REGISTERED_CONTROLLER_COMMAND_H
