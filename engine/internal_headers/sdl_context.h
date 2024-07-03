#ifndef SDL_CONTEXT_H
#define SDL_CONTEXT_H

#include <SDL.h>
#include <optional>
#include <string_view>

namespace roingine {
	class SDLContext final {
		SDL_Window   *m_rpWindow;
		SDL_GLContext m_rpContext;
		int           m_WindowWidth{}, m_WindowHeight{};

	public:
		SDLContext(
		        std::string_view windowTitle, int windowWidth, int windowHeight, std::optional<int> windowX,
		        std::optional<int> windowY
		);

		~SDLContext();

		SDLContext(SDLContext &&) noexcept = delete;

		SDLContext(SDLContext const &) = delete;

		SDLContext &operator=(SDLContext &&) noexcept = delete;

		SDLContext &operator=(SDLContext const &) = delete;

		void SwapWindow() const;
	};
}// namespace roingine

#endif//SDL_CONTEXT_H
