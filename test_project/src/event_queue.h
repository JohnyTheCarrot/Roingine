#ifndef GAME_EVENT_QUEUE_H
#define GAME_EVENT_QUEUE_H

#include <glm/vec2.hpp>
#include <roingine/event_queue.h>

namespace bomberman::event_queue {
	enum class EventType {
		BombPlaceRequest,
		BombDetonated,
	};
}

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::BombDetonated>
        final {
	struct Data_t final {
		glm::vec2 position;
		// TODO: placed by whom, so we can determine the range of the explosion
	};
};// namespace roingine

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::BombPlaceRequest>
        final {
	struct Data_t final {
		glm::vec2 position;
		// TODO: by who?
	};
};

namespace bomberman::event_queue {
	using BombPlaceRequestData = roingine::EventTypeData<EventType, EventType::BombPlaceRequest>::Data_t;
	using BombDetonatedData    = roingine::EventTypeData<EventType, EventType::BombDetonated>::Data_t;

	using EventQueue = roingine::EventQueue<EventType, BombDetonatedData, BombPlaceRequestData>;
}// namespace bomberman::event_queue

#endif//GAME_EVENT_QUEUE_H
