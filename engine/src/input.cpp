#include "input_internal.h"

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
}// namespace roingine
