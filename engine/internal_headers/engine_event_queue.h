#ifndef ENGINE_EVENT_QUEUE_H
#define ENGINE_EVENT_QUEUE_H

#include <roingine/event_queue.h>

#include <variant>

namespace roingine {
	//todo
	namespace event_queue {
		enum class EventType { PlaySoundRequest, Banana };
	}

	template<>
	struct EventTypeData<event_queue::EventType, event_queue::EventType::PlaySoundRequest> final {
		struct Data_t final {
			int a;
		};
	};

	namespace event_queue {
		using PlaySoundRequestData = typename EventTypeData<EventType, EventType::PlaySoundRequest>::Data_t;

		using EventQueue = EventQueue<EventType, PlaySoundRequestData>;
	}// namespace event_queue

}// namespace roingine

#endif// ENGINE_EVENT_QUEUE_H
