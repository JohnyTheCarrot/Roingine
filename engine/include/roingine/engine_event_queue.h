#ifndef ENGINE_EVENT_QUEUE_H
#define ENGINE_EVENT_QUEUE_H

#include <roingine/event_queue.h>

namespace roingine {
	namespace event_queue {
		enum class EventType { CleanShutdown };
	}

	template<>
	struct EventTypeData<event_queue::EventType, event_queue::EventType::CleanShutdown> final {
		struct Data_t final {
		};
	};

	namespace event_queue {
		using CleanShutdownData = typename EventTypeData<EventType, EventType::CleanShutdown>::Data_t;

		using EventQueue = EventQueue<EventType, CleanShutdownData>;
	}// namespace event_queue
}// namespace roingine

#endif// ENGINE_EVENT_QUEUE_H
