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

	Input::Input()  = default;
	Input::~Input() = default;
}// namespace roingine
