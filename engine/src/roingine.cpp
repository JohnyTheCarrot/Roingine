#include "game_time_impl.h"
#include "input_impl.h"
#include <roingine/game_time.h>
#include <roingine/input.h>
#include <roingine/roingine.h>
// clang-format off
#include <windows.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
// clang-format on
#include <SDL.h>
#include <cassert>
#include <roingine/scene_manager.h>
#include <stdexcept>
#include <string>

namespace roingine {
	class Engine::Impl final {
	public:
		Impl(Engine::Settings const &settings) {
			if (SDL_Init(SDL_INIT_VIDEO) != 0) {
				std::runtime_error{std::string{"SDL_Init error: "} + SDL_GetError()};
			}

			int const windowPosX{settings.windowPosX.value_or(SDL_WINDOWPOS_CENTERED)};
			int const windowPosY{settings.windowPosY.value_or(SDL_WINDOWPOS_CENTERED)};

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

			m_rpWindow = SDL_CreateWindow(
			        settings.gameTitle.data(), windowPosX, windowPosY, settings.windowWidth, settings.windowHeight,
			        SDL_WINDOW_OPENGL
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
				char const *errStr{reinterpret_cast<char const *>(gluErrorString(error))};
				throw std::runtime_error{std::string{"OpenGL error: "} + errStr};
			}

			glOrtho(0, settings.windowWidth, 0, settings.windowHeight, -1, 1);
			glViewport(0, 0, settings.windowWidth, settings.windowHeight);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			if (auto const error{glGetError()}; error != GL_NO_ERROR) {
				char const *errStr{reinterpret_cast<char const *>(gluErrorString(error))};
				throw std::runtime_error{std::string{"OpenGL error: "} + errStr};
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glClearColor(0.f, 0.f, 0.f, 0.f);
		}

		~Impl() {
			SDL_DestroyWindow(m_rpWindow);
			SDL_Quit();
		}

		void Run() {
#ifndef __EMSCRIPTEN__
			while (!m_Quit) RunOneFrame();
#else
#error TODO
#endif
		}

	private:
		SDL_Window   *m_rpWindow;
		SDL_GLContext m_rpContext;
		bool        m_Quit{false};

		void RunOneFrame() {
			GameTime &gameTime{GameTime::GetInstance()};

			gameTime.m_pImpl->EndDeltaTimeMeasurement();
			gameTime.m_pImpl->StartDeltaTimeMeasurement();

			static GameTime::DurationPrecision accumulator{0};

			accumulator += gameTime.GetDeltaTime();

			m_Quit = !Input::GetInstance().m_pImpl->ProcessInput();
			auto &sceneManager{SceneManager::GetInstance()};

			while (accumulator >= GameTime::Impl::FIXED_TIME_DELTA) {
				accumulator -= GameTime::Impl::FIXED_TIME_DELTA;
				sceneManager.FixedUpdate();
			}

			sceneManager.Update();
			sceneManager.Render();

			SDL_GL_SwapWindow(m_rpWindow);

			gameTime.m_pImpl->Sleep();
		}
	};

	Engine::Engine(Engine::Settings const &settings)
	    : m_pImpl{std::make_unique<Engine::Impl>(settings)} {
	}

	Engine::~Engine() = default;

	Engine::Engine(Engine &&) = default;

	Engine &Engine::operator=(Engine &&) = default;

	void Engine::Run() {
		m_pImpl->Run();
	}
}// namespace roingine
