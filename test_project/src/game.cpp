#include "game.h"

#include "components/moving_entity.h"
#include "components/player.h"

#include <roingine/components/camera.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>

namespace bomberman {
	PlayerAndCamera::PlayerAndCamera(
	        roingine::Scene &scene, bool hasKeyboardSupport, int viewX, int viewY, int viewWidth, int viewHeight
	) {
		auto cameraObject{scene.AddGameObject()};
		auto playerObject{scene.AddGameObject()};

		auto &cameraTransform{cameraObject.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, 0.f)};

		m_rpCamera = &cameraObject.AddComponent<roingine::Camera>(viewX, viewY, viewWidth, viewHeight);

		playerObject.AddComponent<roingine::Transform>(glm::vec2{50.f, 50.f}, 0.f);
		playerObject.AddComponent<roingine::RectRenderer>(50.f, 50.f);
		auto *rpMovingEntity{&playerObject.AddComponent<MovingEntity>(100.f)};
		m_rpPlayer = &playerObject.AddComponent<Player>(cameraTransform, rpMovingEntity, hasKeyboardSupport);
	}

	roingine::Camera &PlayerAndCamera::GetCamera() const {
		return *m_rpCamera;
	}

	Player &PlayerAndCamera::GetPlayer() const {
		return *m_rpPlayer;
	}

	void Game::ControllerConnectedHandler(roingine::event_queue::ControllerConnectedData const &data) {
		if (!m_PlayerOne.GetPlayer().HasConnectedController()) {
			m_PlayerOne.GetPlayer().TieToController(data.rpController);
			return;
		}

		if (m_PlayerTwo.has_value())
			return;// max 2 players

		m_PlayerOne.GetCamera().SetView(0, 0, m_WindowWidth / 2, m_WindowHeight);
		auto const scene{roingine::SceneManager::GetInstance().GetActive()};
		m_PlayerTwo.emplace(*scene, false, m_WindowWidth / 2, 0, m_WindowWidth / 2, m_WindowHeight);
		m_PlayerTwo->GetPlayer().TieToController(data.rpController);
	}

	void Game::ControllerDisconnectedHandler(roingine::event_queue::ControllerDisconnectedData const &data) {
		if (m_PlayerOne.GetPlayer().HasSpecificController(data.rpController)) {
			m_PlayerOne.GetPlayer().DisconnectController();
			return;
		}

		if (m_PlayerTwo.has_value() && m_PlayerTwo->GetPlayer().HasSpecificController(data.rpController)) {
			m_PlayerTwo->GetCamera().GetGameObject().Destroy();
			m_PlayerTwo->GetPlayer().GetGameObject().Destroy();
			m_PlayerTwo.reset();
			m_PlayerOne.GetCamera().SetView(0, 0, m_WindowWidth, m_WindowHeight);
		}
	}

	Game::Game(roingine::Scene &&scene, int windowWidth, int windowHeight)
	    : m_PlayerOne{scene, true, 0, 0, windowWidth, windowHeight}
	    , m_ControllerConnectedHandler{roingine::event_queue::EventQueue::GetInstance()
	                                           .AttachEventHandler<
	                                                   roingine::event_queue::EventType::ControllerConnected>(
	                                                   [this](auto const &data) { ControllerConnectedHandler(data); }
	                                           )}
	    , m_ControllerDisconnectedHandler{roingine::event_queue::EventQueue::GetInstance()
	                                              .AttachEventHandler<
	                                                      roingine::event_queue::EventType::ControllerDisconnected>(
	                                                      [this](auto const &data) {
		                                                      ControllerDisconnectedHandler(data);
	                                                      }
	                                              )}
	    , m_WindowWidth{windowWidth}
	    , m_WindowHeight{windowHeight} {
		roingine::SceneManager::GetInstance().SetActive(std::move(scene));
	}
}// namespace bomberman
