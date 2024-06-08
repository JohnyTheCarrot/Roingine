#ifndef UNIQUE_SDL_SURFACE_H
#define UNIQUE_SDL_SURFACE_H

#include <memory>

struct SDL_Surface;

namespace roingine {
	class SDLSurfaceDestroyer final {
	public:
		void operator()(SDL_Surface *surface);
	};

	using UniqueSDLSurface = std::unique_ptr<SDL_Surface, SDLSurfaceDestroyer>;
}// namespace roingine

#endif// UNIQUE_SDL_SURFACE_H
