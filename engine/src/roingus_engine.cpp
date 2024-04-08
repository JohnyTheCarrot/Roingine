#include "roingus.h"
#include <SDL.h>
#include <stdexcept>
#include <string>

namespace roingus {
	class Engine::Impl final {
	public:
		Impl(Engine::Settings const &settings) {
			if (SDL_Init(SDL_INIT_VIDEO) != 0) {
				std::runtime_error{std::string{"SDL_Init error: "} + SDL_GetError()};
			}

			int const windowPosX{settings.windowPosX.value_or(SDL_WINDOWPOS_CENTERED)};
			int const windowPosY{settings.windowPosY.value_or(SDL_WINDOWPOS_CENTERED)};

			m_Window = SDL_CreateWindow(
			        settings.gameTitle.data(), windowPosX, windowPosY, settings.windowWidth, settings.windowHeight,
			        SDL_WINDOW_OPENGL
			);

			if (m_Window == nullptr) {
				throw std::runtime_error{std::string{"SDL_CreateWindow error: "} + SDL_GetError()};
			}
		}

		~Impl() {
			SDL_DestroyWindow(m_Window);
			SDL_Quit();
		}

	private:
		SDL_Window *m_Window;
	};

	Engine::Engine(Engine::Settings const &settings)
	    : m_pImpl{std::make_unique<Engine::Impl>(settings)} {
	}

	Engine::~Engine() = default;

	Engine::Engine(Engine &&) = default;

	Engine &Engine::operator=(Engine &&) = default;
}// namespace roingus
