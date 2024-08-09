#ifndef CONTROLLER_IMPL_H
#define CONTROLLER_IMPL_H

#include "unique_sdl_game_controller.h"
#include <array>
#include <optional>
#include <roingine/controller.h>
#include <string>
#include <vector>

namespace roingine {
	enum class SDLControllerButton : int {
		A             = SDL_CONTROLLER_BUTTON_A,
		B             = SDL_CONTROLLER_BUTTON_B,
		X             = SDL_CONTROLLER_BUTTON_X,
		Y             = SDL_CONTROLLER_BUTTON_Y,
		Back          = SDL_CONTROLLER_BUTTON_BACK,
		Guide         = SDL_CONTROLLER_BUTTON_GUIDE,
		Start         = SDL_CONTROLLER_BUTTON_START,
		LeftStick     = SDL_CONTROLLER_BUTTON_LEFTSTICK,
		RightStick    = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
		LeftShoulder  = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		DPadUp        = SDL_CONTROLLER_BUTTON_DPAD_UP,
		DPadDown      = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		DPadLeft      = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		DPadRight     = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	};

	[[nodiscard]]
	SDLControllerButton ToSDLControllerButton(ControllerButton button);

	[[nodiscard]]
	std::optional<ControllerButton> SDLToControllerButton(SDLControllerButton button);

	[[nodiscard]]
	ButtonState ToButtonState(bool pressed);

	class Controller::Impl final {
		UniqueSDLGameController                            m_pController;
		std::array<ButtonState, SDL_CONTROLLER_BUTTON_MAX> m_ButtonStates{};
		std::vector<ControllerCommand>                     m_Commands;
		bool                                               m_MarkedForDeletion{false};

		[[nodiscard]]
		ButtonState MergeButtonStates(ControllerButton button, ButtonState state1, ButtonState state2);

		void ExecuteCommandsForButtonState(ControllerButton button, ButtonState buttonState);

	public:
		explicit Impl(int controllerIndex);

		[[nodiscard]]
		int GetInstanceID() const;

		[[nodiscard]]
		std::string GetSerial() const;

		void MarkForDeletion();

		[[nodiscard]]
		bool IsMarkedForDeletion() const;

		void Update();

		void RegisterCommand(ControllerButton button, ButtonState buttonState, std::unique_ptr<Command> pCommand);

		void RemoveCommand(ControllerButton button, ButtonState buttonState, Command *pCommand);

		[[nodiscard]]
		ButtonState GetButtonState(ControllerButton button) const;
	};
}// namespace roingine

#endif//CONTROLLER_IMPL_H
