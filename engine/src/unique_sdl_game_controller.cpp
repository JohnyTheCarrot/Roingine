#include "unique_sdl_game_controller.h"
#include <iostream>

namespace roingine {
	void SDLGameControllerDestroyer::operator()(SDL_GameController *pController) const {
		std::cout << "Closing controller with name: " << SDL_GameControllerName(pController) << std::endl;
		SDL_GameControllerClose(pController);
	}
}// namespace roingine
