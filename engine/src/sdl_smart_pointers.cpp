#include "sdl_smart_pointers.h"
#include <SDL.h>

namespace roingine {
	void SDLSurfaceDestroyer::operator()(SDL_Surface *surface) {
		SDL_FreeSurface(surface);
	}

	void SDLPixelFormatDestroyer::operator()(SDL_PixelFormat *format) {
		SDL_FreeFormat(format);
	}
}// namespace roingine
