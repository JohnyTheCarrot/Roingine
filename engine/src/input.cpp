#include "input_impl.h"

#include <SDL_events.h>

namespace roingine {
	bool Input::Impl::ProcessInput() {
		// TODO

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return false;

			// TODO: ImGui process event
		}


		return true;
	}

	bool Input::Impl::IsPressingAssignmentButton() const {
		static bool    lastPressed{false};
		uint8_t const *pKeyboardState = SDL_GetKeyboardState(nullptr);

		auto const newPressed{!lastPressed && pKeyboardState[SDL_SCANCODE_K]};
		lastPressed = pKeyboardState[SDL_SCANCODE_K];

		return newPressed;
	}

	bool Input::IsPressingAssignmentButton() const {
		return m_pImpl->IsPressingAssignmentButton();
	}

	Input::Input()  = default;
	Input::~Input() = default;
}// namespace roingine
