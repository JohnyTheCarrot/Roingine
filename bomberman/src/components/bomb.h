#ifndef BOMB_H
#define BOMB_H

#include "../event_queue.h"

#include <glm/vec2.hpp>
#include <roingine/components/component.h>
#include <roingine/event_queue.h>
#include <roingine/gameobject.h>

namespace bomberman {
	struct PlayerInfo;
}

namespace bomberman {
	class Bomb final : public roingine::Component {
		PlayerInfo                                           *m_rpBomber;
		roingine::EventHandlerHandle<event_queue::EventQueue> m_hExplosionHandler;
		int                                                   m_BombId{};

		void OnCaughtInExplosion(event_queue::ExplosionData const &data);

	public:
		Bomb(roingine::GameObject gameObject, glm::vec2 location, PlayerInfo &bomber);

		void Explode();

		[[nodiscard]]
		int GetBombId() const;
	};
}// namespace bomberman

#endif//BOMB_H
