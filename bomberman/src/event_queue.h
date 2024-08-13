#ifndef GAME_EVENT_QUEUE_H
#define GAME_EVENT_QUEUE_H

#include <glm/vec2.hpp>
#include <roingine/event_queue.h>

namespace bomberman {
	struct PlayerInfo;
}

namespace roingine {
	class Controller;
}

namespace bomberman::event_queue {
	enum class EventType {
		BombPlaceRequest,
		DetonateLastBombRequest,
		BombDetonated,
		Explosion,
		PlayerControllerConnected,
		PlayerControllerDisconnected,
		EnemyDied,
		PlayerDied,
		DoorUsed,
	};
}

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::BombDetonated>
        final {
	struct Data_t final {
		bomberman::PlayerInfo *rpBomber;
		glm::vec2              position;
		int                    bombId;
	};
};

template<>
struct roingine::EventTypeData<
        bomberman::event_queue::EventType, bomberman::event_queue::EventType::DetonateLastBombRequest>
        final {
	struct Data_t final {
		bomberman::PlayerInfo *rpBomber;
	};
};

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::BombPlaceRequest>
        final {
	struct Data_t final {
		bomberman::PlayerInfo *rpBomber;
		glm::vec2              position;
	};
};

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::Explosion> final {
	struct Data_t final {
		glm::vec2 horizontalStart;
		glm::vec2 horizontalEnd;
		glm::vec2 verticalStart;
		glm::vec2 verticalEnd;

		[[nodiscard]]
		bool IsInExplosion(glm::vec2 position) const {
			return (position.x >= horizontalStart.x && position.y >= horizontalStart.y &&
			        position.x <= horizontalEnd.x && position.y <= horizontalEnd.y) ||
			       (position.x >= verticalStart.x && position.y >= verticalStart.y && position.x <= verticalEnd.x &&
			        position.y <= verticalEnd.y);
		}
	};
};

template<>
struct roingine::EventTypeData<
        bomberman::event_queue::EventType, bomberman::event_queue::EventType::PlayerControllerConnected>
        final {
	struct Data_t final {
		Controller *rpController;
		bool        isPlayerOne;
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

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::EnemyDied> final {
	struct Data_t final {
		int score;
	};
};

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::PlayerDied> final {
	struct Data_t final {
		bool isPlayerOne;
	};
};

template<>
struct roingine::EventTypeData<bomberman::event_queue::EventType, bomberman::event_queue::EventType::DoorUsed> final {
	struct Data_t final {};
};

namespace bomberman::event_queue {
	using BombPlaceRequestData    = roingine::EventTypeData<EventType, EventType::BombPlaceRequest>::Data_t;
	using DetonateLastBombRequest = roingine::EventTypeData<EventType, EventType::DetonateLastBombRequest>::Data_t;
	using BombDetonatedData       = roingine::EventTypeData<EventType, EventType::BombDetonated>::Data_t;
	using ControllerConnectedData = roingine::EventTypeData<EventType, EventType::PlayerControllerConnected>::Data_t;
	using ControllerDisconnectedData =
	        roingine::EventTypeData<EventType, EventType::PlayerControllerDisconnected>::Data_t;
	using ExplosionData  = roingine::EventTypeData<EventType, EventType::Explosion>::Data_t;
	using EnemyDiedData  = roingine::EventTypeData<EventType, EventType::EnemyDied>::Data_t;
	using PlayerDiedData = roingine::EventTypeData<EventType, EventType::PlayerDied>::Data_t;
	using DoorUsedData   = roingine::EventTypeData<EventType, EventType::DoorUsed>::Data_t;

	using EventQueue = roingine::EventQueue<
	        EventType, BombDetonatedData, DetonateLastBombRequest, BombPlaceRequestData, ControllerConnectedData,
	        ControllerDisconnectedData, ExplosionData, EnemyDiedData, PlayerDiedData, DoorUsedData>;
}// namespace bomberman::event_queue

#endif//GAME_EVENT_QUEUE_H
