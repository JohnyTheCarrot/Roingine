#include <roingine/game_time.h>
#include <roingine/keyboard_input.h>
#include <roingine/roingine.h>
// clang-format off
#include <windows.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
// clang-format on
#include "roingine/controller.h"
#include "sdl_context.h"


#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <roingine/engine_event_queue.h>
#include <roingine/event_queue.h>
#include <roingine/game_info.h>
#include <roingine/scene_manager.h>
#include <roingine/service_locator.h>

namespace roingine {
	class Engine::Impl final {
	public:
		Impl(std::string_view windowTitle, int windowWidth, int windowHeight, std::optional<int> windowX,
		     std::optional<int> windowY)
		    : m_Context{windowTitle, windowWidth, windowHeight, windowX, windowY} {
		}

		~Impl() = default;

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
		SDLContext                               m_Context;
		std::vector<std::unique_ptr<Controller>> m_Controllers;

		void RunOneFrame(std::function<void()> const &fn) {
			GameTime &gameTime{GameTime::GetInstance()};

			gameTime.StartDeltaTimeMeasurement();

			static GameTime::DurationPrecision accumulator{0};

			accumulator += gameTime.GetDeltaTime();

			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT:
						GameInfo::GetInstance().QuitGame();
						break;
					case SDL_CONTROLLERDEVICEADDED: {
						auto controller{std::make_unique<Controller>(event.cdevice.which)};
						event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::ControllerConnected>(
						        controller.get()
						);
						m_Controllers.emplace_back(std::move(controller));
						break;
					}
					case SDL_CONTROLLERDEVICEREMOVED: {
						auto it{std::ranges::find_if(
						        m_Controllers, [which = event.cdevice.which](auto const &pController
						                       ) { return pController->GetInstanceID() == which; }
						)};

						if (it == m_Controllers.end())
							break;

						event_queue::EventQueue::GetInstance()
						        .FireEvent<event_queue::EventType::ControllerDisconnected>(it->get());

						(*it)->MarkForDeletion();
						break;
					}
				}
			}

			for (auto const &controller: m_Controllers) { controller->Update(); }
			KeyboardInput::GetService().ProcessInput();
			auto const &sceneManager{SceneManager::GetInstance()};

			while (accumulator >= GameTime::FIXED_TIME_DELTA) {
				accumulator -= GameTime::FIXED_TIME_DELTA;
				sceneManager.FixedUpdate();
			}

			sceneManager.PreUpdate();
			sceneManager.Update();
			event_queue::EventQueue::GetInstance().Update();
			sceneManager.PostUpdate();
			fn();

			std::erase_if(m_Controllers, [](auto const &pController) { return pController->IsMarkedForDeletion(); });

			glClear(GL_COLOR_BUFFER_BIT);
			sceneManager.RenderFromCameras();

			m_Context.SwapWindow();

			gameTime.Sleep();
			gameTime.EndDeltaTimeMeasurement();
		}
	};

	Engine::Engine(
	        std::string_view windowTitle, int windowWidth, int windowHeight, std::optional<int> windowX,
	        std::optional<int> windowY
	)
	    : m_pImpl{std::make_unique<Impl>(windowTitle, windowWidth, windowHeight, windowX, windowY)} {
	}

	Engine::~Engine() {
		// Due to SceneManager being a singleton, we need to unload the scene before the engine is destroyed.
		SceneManager::GetInstance().UnloadScene(true);
	}

	Engine::Engine(Engine &&) noexcept = default;

	Engine &Engine::operator=(Engine &&) noexcept = default;

	void Engine::Run(std::function<void()> const &fn) {
		m_pImpl->Run(fn);
	}
}// namespace roingine
