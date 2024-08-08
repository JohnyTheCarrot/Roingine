#include "game.h"

#include "components/player.h"
#include "level.h"
#include "player_info.h"

namespace bomberman {
	void Game::OnControllerConnected(roingine::event_queue::ControllerConnectedData const &data) {
		if (!m_UnassignedController1.has_value()) {
			m_UnassignedController1.emplace(data.rpController, m_Level, [this]() { m_UnassignedController1.reset(); });

			return;
		}

		if (!m_UnassignedController2.has_value()) {
			m_UnassignedController2.emplace(data.rpController, m_Level, [this]() { m_UnassignedController2.reset(); });
		}
	}

	void Game::OnControllerDisconnected(roingine::event_queue::ControllerDisconnectedData const &data) {
		if (m_Level.DoesPlayer1OwnController(*data.rpController)) {
			m_Level.SetPlayer1Controller(nullptr);
			return;
		}

		if (m_Level.DoesPlayer2OwnController(*data.rpController)) {
			m_Level.SetPlayer2Controller(nullptr);
		}
	}

	Game::Game(roingine::Scene &&scene, int windowWidth, int windowHeight)
	    : m_Level{scene, LevelLoadInfo{windowWidth, windowHeight}}
	    , m_hControllerConnected{roingine::event_queue::EventQueue::GetInstance()
	                                     .AttachEventHandler<roingine::event_queue::EventType::ControllerConnected>(
	                                             [this](auto const &data) { OnControllerConnected(data); }
	                                     )}
	    , m_hControllerDisconnected{
	              roingine::event_queue::EventQueue::GetInstance()
	                      .AttachEventHandler<roingine::event_queue::EventType::ControllerDisconnected>(
	                              [this](auto const &data) { OnControllerDisconnected(data); }
	                      )
	      } {
	}
}// namespace bomberman
