#ifndef GAME_EVENT_QUEUE_H
#define GAME_EVENT_QUEUE_H

#include <glm/vec2.hpp>
#include <roingine/engine_event_queue.h>
#include <roingine/event_queue.h>

namespace bomberman::event_queue {
	enum class EventType {
		BombPlaceRequest,
		BombDetonated,
		PlayerControllerConnected,
		PlayerControllerDisconnected,
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

template<>
struct roingine::EventTypeData<
        bomberman::event_queue::EventType, bomberman::event_queue::EventType::PlayerControllerConnected>
        final {
	struct Data_t final {
		Controller *rpController;
		bool        playerOne;
	};
};

template<>
struct roingine::EventTypeData<
        bomberman::event_queue::EventType, bomberman::event_queue::EventType::PlayerControllerDisconnected>
        final {
	struct Data_t final {
		bool playerOne;
	};
};

namespace bomberman::event_queue {
	using BombPlaceRequestData    = roingine::EventTypeData<EventType, EventType::BombPlaceRequest>::Data_t;
	using BombDetonatedData       = roingine::EventTypeData<EventType, EventType::BombDetonated>::Data_t;
	using ControllerConnectedData = roingine::EventTypeData<EventType, EventType::PlayerControllerConnected>::Data_t;
	using ControllerDisconnectedData =
	        roingine::EventTypeData<EventType, EventType::PlayerControllerDisconnected>::Data_t;

	using EventQueue = roingine::EventQueue<
	        EventType, BombDetonatedData, BombPlaceRequestData, ControllerConnectedData, ControllerDisconnectedData>;
}// namespace bomberman::event_queue

#endif//GAME_EVENT_QUEUE_H
