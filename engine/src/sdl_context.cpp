#include "sdl_context.h"

// clang-format off
#include <windows.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
// clang-format on
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdexcept>

namespace roingine {
	SDLContext::SDLContext(
	        std::string_view windowTitle, int windowWidth, int windowHeight, std::optional<int> windowX,
	        std::optional<int> windowY
	) {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) {
			throw std::runtime_error{std::string{"SDL_Init error: "} + SDL_GetError()};
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			throw std::runtime_error{std::string{"IMG_Init error: "} + IMG_GetError()};
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
			throw std::runtime_error{std::string{"SDL_mixer could not initialize! SDL_mixer Error: "} + SDL_GetError()};
		}

		int const windowPosX{windowX.value_or(SDL_WINDOWPOS_CENTERED)};
		int const windowPosY{windowY.value_or(SDL_WINDOWPOS_CENTERED)};

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		m_rpWindow = SDL_CreateWindow(
		        windowTitle.data(), windowPosX, windowPosY, windowWidth, windowHeight, SDL_WINDOW_OPENGL
		);

		if (m_rpWindow == nullptr) {
			throw std::runtime_error{std::string{"SDL_CreateWindow error: "} + SDL_GetError()};
		}

		m_rpContext = SDL_GL_CreateContext(m_rpWindow);

		if (m_rpContext == nullptr) {
			throw std::runtime_error{std::string{"SDL_GL_CreateContext() failed: "} + SDL_GetError()};
		}

		if (SDL_GL_SetSwapInterval(1) < 0) {
			throw std::runtime_error{std::string{"SDL_GL_SetSwapInterval() failed: "} + SDL_GetError()};
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (auto const error{glGetError()}; error != GL_NO_ERROR) {
			auto const *errStr{reinterpret_cast<char const *>(gluErrorString(error))};
			throw std::runtime_error{std::string{"OpenGL error: "} + errStr};
		}

		gluOrtho2D(0, windowWidth, 0, windowHeight);
		glViewport(0, 0, windowWidth, windowHeight);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		if (auto const error{glGetError()}; error != GL_NO_ERROR) {
			auto const errStr{reinterpret_cast<char const *>(gluErrorString(error))};
			throw std::runtime_error{std::string{"OpenGL error: "} + errStr};
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClearColor(0.f, 0.f, 0.f, 0.f);
	}

	SDLContext::~SDLContext() {
		std::cout << "Destroying SDL context" << std::endl;
		SDL_DestroyWindow(m_rpWindow);
		SDL_Quit();
		IMG_Quit();
	}

	void SDLContext::SwapWindow() const {
		SDL_GL_SwapWindow(m_rpWindow);
	}
}// namespace roingine
