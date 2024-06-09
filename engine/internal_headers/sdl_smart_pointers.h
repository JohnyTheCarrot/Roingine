#ifndef SDL_SMART_POINTERS_H
#define SDL_SMART_POINTERS_H

#include <memory>

struct SDL_Surface;

struct SDL_PixelFormat;

namespace roingine {
	class SDLSurfaceDestroyer final {
	public:
		void operator()(SDL_Surface *surface);
	};

	using UniqueSDLSurface = std::unique_ptr<SDL_Surface, SDLSurfaceDestroyer>;

	class SDLPixelFormatDestroyer final {
	public:
		void operator()(SDL_PixelFormat *format);
	};

	using UniqueSDLPixelFormat = std::unique_ptr<SDL_PixelFormat, SDLPixelFormatDestroyer>;
}// namespace roingine

#endif// SDL_SMART_POINTERS_H
