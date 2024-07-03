#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include <roingine/commands/command.h>

namespace roingine {
	enum class ControllerButton {
		A,
		B,
		X,
		Y,
		Back,
		Guide,
		Start,
		LeftStick,
		RightStick,
		LeftShoulder,
		RightShoulder,
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
	};

	enum class ButtonState { Pressed, NotPressed, Held };

	struct ControllerCommand {
		ControllerButton         button;
		ButtonState              buttonState;
		std::unique_ptr<Command> pCommand;
	};

	class Controller final {
		class Impl;
		std::unique_ptr<Impl> m_pImpl;

	public:
		explicit Controller(int controllerIndex);

		~Controller();

		Controller(Controller &&) noexcept;

		Controller(Controller const &) = delete;

		Controller &operator=(Controller &&) noexcept;

		Controller &operator=(Controller const &) = delete;

		[[nodiscard]]
		int GetInstanceID() const;

		void Update() const;

		void RegisterCommand(ControllerButton button, ButtonState buttonState, std::unique_ptr<Command> pCommand) const;

		void RemoveCommand(ControllerButton button, ButtonState buttonState, Command *pCommand) const;
	};
}// namespace roingine

#endif//CONTROLLER_H