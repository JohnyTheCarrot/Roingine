#include "unique_sdl_surface.h"
#include <SDL.h>

namespace roingine {
	void SDLSurfaceDestroyer::operator()(SDL_Surface *surface) {
		SDL_FreeSurface(surface);
	}
}// namespace roingine
