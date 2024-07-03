#ifndef ENGINE_EVENT_QUEUE_H
#define ENGINE_EVENT_QUEUE_H

#include <roingine/event_queue.h>

namespace roingine {
	class Controller;

	namespace event_queue {
		enum class EventType { CleanShutdown, ControllerConnected, ControllerDisconnected };
	}

	template<>
	struct EventTypeData<event_queue::EventType, event_queue::EventType::CleanShutdown> final {
		struct Data_t final {};
	};

	template<>
	struct EventTypeData<event_queue::EventType, event_queue::EventType::ControllerConnected> final {
		struct Data_t final {
			Controller *rpController;
		};
	};

	template<>
	struct EventTypeData<event_queue::EventType, event_queue::EventType::ControllerDisconnected> final {
		struct Data_t final {
			// Only for the purpose of matching the controller, do not use this pointer
			Controller *rpController;
		};
	};

	namespace event_queue {
		using CleanShutdownData          = EventTypeData<EventType, EventType::CleanShutdown>::Data_t;
		using ControllerConnectedData    = EventTypeData<EventType, EventType::ControllerConnected>::Data_t;
		using ControllerDisconnectedData = EventTypeData<EventType, EventType::ControllerDisconnected>::Data_t;

		using EventQueue =
		        EventQueue<EventType, CleanShutdownData, ControllerConnectedData, ControllerDisconnectedData>;
	}// namespace event_queue
}// namespace roingine

#endif// ENGINE_EVENT_QUEUE_H
