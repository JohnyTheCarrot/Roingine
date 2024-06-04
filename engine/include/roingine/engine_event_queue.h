#ifndef ENGINE_EVENT_QUEUE_H
#define ENGINE_EVENT_QUEUE_H

#include <roingine/component_init_types.h>
#include <roingine/event_queue.h>
#include <string>

namespace roingine {
	namespace event_queue {
		enum class EventType { CleanShutdown, ScriptEvent };
	}

	template<>
	struct EventTypeData<event_queue::EventType, event_queue::EventType::CleanShutdown> final {
		struct Data_t final {
		};
	};

	template<>
	struct EventTypeData<event_queue::EventType, event_queue::EventType::ScriptEvent> final {
		struct Data_t final {
			std::string         eventName;
			std::vector<JSData> data;
		};
	};

	namespace event_queue {
		using CleanShutdownData = typename EventTypeData<EventType, EventType::CleanShutdown>::Data_t;
		using ScriptEventData   = typename EventTypeData<EventType, EventType::ScriptEvent>::Data_t;

		using EventQueue = EventQueue<EventType, CleanShutdownData, ScriptEventData>;
	}// namespace event_queue
}// namespace roingine

#endif// ENGINE_EVENT_QUEUE_H
