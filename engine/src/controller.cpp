#include "controller_impl.h"

#include <algorithm>
#include <iostream>
#include <roingine/commands/command.h>
#include <stdexcept>

using namespace std::literals::string_literals;

namespace roingine {
	SDLControllerButton ToSDLControllerButton(ControllerButton button) {
		switch (button) {
			case ControllerButton::A:
				return SDLControllerButton::A;
			case ControllerButton::B:
				return SDLControllerButton::B;
			case ControllerButton::X:
				return SDLControllerButton::X;
			case ControllerButton::Y:
				return SDLControllerButton::Y;
			case ControllerButton::Back:
				return SDLControllerButton::Back;
			case ControllerButton::Guide:
				return SDLControllerButton::Guide;
			case ControllerButton::Start:
				return SDLControllerButton::Start;
			case ControllerButton::LeftStick:
				return SDLControllerButton::LeftStick;
			case ControllerButton::RightStick:
				return SDLControllerButton::RightStick;
			case ControllerButton::LeftShoulder:
				return SDLControllerButton::LeftShoulder;
			case ControllerButton::RightShoulder:
				return SDLControllerButton::RightShoulder;
			case ControllerButton::DPadUp:
				return SDLControllerButton::DPadUp;
			case ControllerButton::DPadDown:
				return SDLControllerButton::DPadDown;
			case ControllerButton::DPadLeft:
				return SDLControllerButton::DPadLeft;
			case ControllerButton::DPadRight:
				return SDLControllerButton::DPadRight;
		}

		throw std::runtime_error{"Invalid controller button"};
	}

	std::optional<ControllerButton> SDLToControllerButton(SDLControllerButton button) {
		switch (button) {
			case SDLControllerButton::A:
				return ControllerButton::A;
			case SDLControllerButton::B:
				return ControllerButton::B;
			case SDLControllerButton::X:
				return ControllerButton::X;
			case SDLControllerButton::Y:
				return ControllerButton::Y;
			case SDLControllerButton::Back:
				return ControllerButton::Back;
			case SDLControllerButton::Guide:
				return ControllerButton::Guide;
			case SDLControllerButton::Start:
				return ControllerButton::Start;
			case SDLControllerButton::LeftStick:
				return ControllerButton::LeftStick;
			case SDLControllerButton::RightStick:
				return ControllerButton::RightStick;
			case SDLControllerButton::LeftShoulder:
				return ControllerButton::LeftShoulder;
			case SDLControllerButton::RightShoulder:
				return ControllerButton::RightShoulder;
			case SDLControllerButton::DPadUp:
				return ControllerButton::DPadUp;
			case SDLControllerButton::DPadDown:
				return ControllerButton::DPadDown;
			case SDLControllerButton::DPadLeft:
				return ControllerButton::DPadLeft;
			case SDLControllerButton::DPadRight:
				return ControllerButton::DPadRight;
		}

		return std::nullopt;
	}

	ButtonState ToButtonState(bool pressed) {
		return pressed ? ButtonState::Pressed : ButtonState::NotPressed;
	}

	ButtonState Controller::Impl::MergeButtonStates(ControllerButton button, ButtonState state1, ButtonState state2) {
		// If the button was pressed between the two states, it is held
		if ((state1 == ButtonState::Pressed || state1 == ButtonState::Held) && state2 == ButtonState::Pressed) {
			ExecuteCommandsForButtonState(button, ButtonState::Held);
			return ButtonState::Held;
		}

		// If the button was not pressed and is now pressed
		if (state1 == ButtonState::NotPressed && state2 == ButtonState::Pressed) {
			ExecuteCommandsForButtonState(button, state2);
			return state2;
		}

		// If the button was pressed and is now not pressed
		if (state1 != ButtonState::NotPressed && state2 == ButtonState::NotPressed) {
			ExecuteCommandsForButtonState(button, state2);
			return state2;
		}

		return state2;
	}

	void Controller::Impl::ExecuteCommandsForButtonState(ControllerButton button, ButtonState buttonState) {
		auto const it{std::ranges::find_if(m_Commands, [button, buttonState](auto const &command) {
			return command.button == button && command.buttonState == buttonState;
		})};

		if (it == m_Commands.end()) {
			return;
		}

		it->pCommand->Execute();
	}

	Controller::Impl::Impl(int controllerIndex)
	    : m_pController{SDL_GameControllerOpen(controllerIndex)} {
		if (m_pController == nullptr) {
			throw std::runtime_error{"Failed to open controller: "s + SDL_GetError()};
		}
		std::ranges::fill(m_ButtonStates, ButtonState::NotPressed);
		std::cout << "Controller opened with name: " << SDL_GameControllerName(m_pController.get()) << '\n';
	}

	int Controller::Impl::GetInstanceID() const {
		auto const joystick{SDL_GameControllerGetJoystick(m_pController.get())};

		return SDL_JoystickInstanceID(joystick);
	}

	std::string Controller::Impl::GetSerial() const {
		auto const joystick{SDL_GameControllerGetJoystick(m_pController.get())};

		auto const *serial{SDL_JoystickGetSerial(joystick)};

		return serial;
	}

	void Controller::Impl::MarkForDeletion() {
		m_MarkedForDeletion = true;
	}

	bool Controller::Impl::IsMarkedForDeletion() const {
		return m_MarkedForDeletion;
	}

	void Controller::Impl::Update() {
		std::ranges::generate(m_ButtonStates, [this, buttonIndex = 0]() mutable {
			auto const button{static_cast<SDL_GameControllerButton>(buttonIndex)};

			auto const pressed{SDL_GameControllerGetButton(m_pController.get(), button) != 0};

			auto const newButtonState{ToButtonState(pressed)};
			auto const oldButtonState{m_ButtonStates[buttonIndex]};
			auto const sdlControllerButton{static_cast<SDLControllerButton>(buttonIndex)};
			auto const controllerButton{SDLToControllerButton(sdlControllerButton)};
			if (!controllerButton.has_value())
				return ButtonState::NotPressed;

			++buttonIndex;
			return MergeButtonStates(controllerButton.value(), oldButtonState, newButtonState);
		});
	}

	void Controller::Impl::RegisterCommand(
	        ControllerButton button, ButtonState buttonState, std::unique_ptr<Command> pCommand
	) {
		m_Commands.emplace_back(button, buttonState, std::move(pCommand));
	}

	void Controller::Impl::RemoveCommand(ControllerButton button, ButtonState buttonState, Command *pCommand) {
		auto const it{std::ranges::find_if(m_Commands, [button, buttonState, pCommand](auto const &command) {
			return command.button == button && command.buttonState == buttonState && command.pCommand.get() == pCommand;
		})};

		if (it == m_Commands.end()) {
			throw std::runtime_error{"Command not found"};
		}

		m_Commands.erase(it);
	}

	Controller::Controller(int controllerIndex)
	    : m_pImpl{std::make_unique<Impl>(controllerIndex)} {
	}

	Controller::~Controller() = default;

	Controller::Controller(Controller &&) noexcept = default;

	Controller &Controller::operator=(Controller &&) noexcept = default;

	int Controller::GetInstanceID() const {
		return m_pImpl->GetInstanceID();
	}

	std::string Controller::GetSerial() const {
		return m_pImpl->GetSerial();
	}

	void Controller::MarkForDeletion() const {
		m_pImpl->MarkForDeletion();
	}

	bool Controller::IsMarkedForDeletion() const {
		return m_pImpl->IsMarkedForDeletion();
	}

	void Controller::Update() const {
		m_pImpl->Update();
	}

	void Controller::RegisterCommand(
	        ControllerButton button, ButtonState buttonState, std::unique_ptr<Command> pCommand
	) const {
		m_pImpl->RegisterCommand(button, buttonState, std::move(pCommand));
	}

	void Controller::RemoveCommand(ControllerButton button, ButtonState buttonState, Command *pCommand) const {
		m_pImpl->RemoveCommand(button, buttonState, pCommand);
	}
}// namespace roingine
