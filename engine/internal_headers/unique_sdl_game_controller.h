#ifndef UNIQUE_SDL_GAME_CONTROLLER_H
#define UNIQUE_SDL_GAME_CONTROLLER_H

#include <SDL_gamecontroller.h>
#include <memory>

namespace roingine {
	class SDLGameControllerDestroyer final {
	public:
		void operator()(SDL_GameController *pController) const;
	};

	using UniqueSDLGameController = std::unique_ptr<SDL_GameController, SDLGameControllerDestroyer>;
}// namespace roingine

#endif//UNIQUE_SDL_GAME_CONTROLLER_H
