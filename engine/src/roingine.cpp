#include "game_time_impl.h"
#include <roingine/game_time.h>
#include <roingine/input.h>
#include <roingine/roingine.h>
// clang-format off
#include <windows.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
// clang-format on
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <roingine/engine_event_queue.h>
#include <roingine/event_queue.h>
#include <roingine/game_info.h>
#include <roingine/scene_manager.h>
#include <roingine/service_locator.h>
#include <stdexcept>
#include <string>

namespace roingine {
	class Engine::Impl final {
	public:
		Impl(std::string_view windowTitle, int windowWidth, int windowHeight, std::optional<int> windowX,
		     std::optional<int> windowY) {
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
				std::runtime_error{std::string{"SDL_Init error: "} + SDL_GetError()};
			}

			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
				std::runtime_error{std::string{"SDL_mixer could not initialize! SDL_mixer Error: "} + SDL_GetError()};
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
				char const *errStr{reinterpret_cast<char const *>(gluErrorString(error))};
				throw std::runtime_error{std::string{"OpenGL error: "} + errStr};
			}

			glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
			glViewport(0, 0, windowWidth, windowHeight);

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

		void Run(std::function<void()> const &fn) {
			auto const &gameInfo{GameInfo::GetInstance()};
			try {
#ifndef __EMSCRIPTEN__
				while (gameInfo.IsRunning()) RunOneFrame(fn);
#else
#error TODO
#endif
			} catch (std::exception const &exception) {
				std::cerr << "[ERROR] " << exception.what() << std::endl;
			} catch (...) { std::cerr << "Unknown error" << std::endl; }
		}

	private:
		SDL_Window   *m_rpWindow;
		SDL_GLContext m_rpContext;

		void RunOneFrame(std::function<void()> const &fn) {
			GameTime &gameTime{GameTime::GetInstance()};

			gameTime.m_pImpl->StartDeltaTimeMeasurement();

			static GameTime::DurationPrecision accumulator{0};

			accumulator += gameTime.GetDeltaTime();

			KeyboardInput::GetService().ProcessInput();
			auto &sceneManager{SceneManager::GetInstance()};

			while (accumulator >= GameTime::Impl::FIXED_TIME_DELTA) {
				accumulator -= GameTime::Impl::FIXED_TIME_DELTA;
				sceneManager.FixedUpdate();
			}

			sceneManager.Update();
			event_queue::EventQueue::GetInstance().Update();
			fn();

			glClear(GL_COLOR_BUFFER_BIT);
			sceneManager.Render();

			SDL_GL_SwapWindow(m_rpWindow);

			gameTime.m_pImpl->Sleep();
			gameTime.m_pImpl->EndDeltaTimeMeasurement();
		}
	};

	Engine::Engine(
	        std::string_view windowTitle, int windowWidth, int windowHeight, std::optional<int> windowX,
	        std::optional<int> windowY
	)
	    : m_pImpl{std::make_unique<Engine::Impl>(windowTitle, windowWidth, windowHeight, windowX, windowY)} {
	}

	Engine::~Engine() = default;

	Engine::Engine(Engine &&) = default;

	Engine &Engine::operator=(Engine &&) = default;

	void Engine::Run(std::function<void()> const &fn) {
		m_pImpl->Run(fn);
	}
}// namespace roingine
