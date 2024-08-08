#ifndef GAME_H
#define GAME_H

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
		Level m_Level;

		roingine::EventHandlerHandle<roingine::event_queue::EventQueue> m_hControllerConnected;
		roingine::EventHandlerHandle<roingine::event_queue::EventQueue> m_hControllerDisconnected;

		std::optional<UnassignedController> m_UnassignedController1;
		std::optional<UnassignedController> m_UnassignedController2;
		roingine::Scene                    *m_rpScene{};

		void OnControllerConnected(roingine::event_queue::ControllerConnectedData const &data);

		void OnControllerDisconnected(roingine::event_queue::ControllerDisconnectedData const &data);

	public:
		Game(roingine::Scene &&scene, int windowWidth, int windowHeight);
	};
}// namespace bomberman

#endif//GAME_H
