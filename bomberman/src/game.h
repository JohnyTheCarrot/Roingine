#ifndef GAME_H
#define GAME_H

#include "event_queue.h"
#include "level.h"
#include "unassigned_controller.h"

#include <roingine/engine_event_queue.h>
#include <roingine/gameobject.h>

namespace roingine {
	class Camera;

	class Transform;
}// namespace roingine

namespace bomberman {
	class Game final {
		roingine::EventHandlerHandle<roingine::event_queue::EventQueue> m_hControllerConnected;
		roingine::EventHandlerHandle<roingine::event_queue::EventQueue> m_hControllerDisconnected;
		roingine::EventHandlerHandle<event_queue::EventQueue>           m_hDoorUsed;

		std::vector<LevelSetupData>         m_LevelSetupData{};
		Level                               m_Level;
		std::optional<UnassignedController> m_UnassignedController1;
		std::optional<UnassignedController> m_UnassignedController2;
		roingine::Scene                    *m_rpScene{};
		int                                 m_WindowWidth;
		int                                 m_WindowHeight;
		std::size_t                         m_CurrentLevelIndex{0};

		void OnControllerConnected(roingine::event_queue::ControllerConnectedData const &data);

		void OnControllerDisconnected(roingine::event_queue::ControllerDisconnectedData const &data) const;

		static std::vector<LevelSetupData> LoadLevelSetupData();

	public:
		Game(int windowWidth, int windowHeight);

		void LoadNextLevel();
	};
}// namespace bomberman

#endif//GAME_H
