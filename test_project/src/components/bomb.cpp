#include "bomb.h"

#include "../event_queue.h"

#include <roingine/components/transform.h>
#include <roingine/game_time.h>

namespace bomberman {
	constexpr float c_DetonationTime{2.f};

	Bomb::Bomb(roingine::GameObject gameObject)
	    : Component{gameObject} {
	}

	void Bomb::Update() {
		m_Age += roingine::GameTime::GetInstance().GetDeltaTime();

		if (m_Age < c_DetonationTime)
			return;

		auto        gameObject{GetGameObject()};
		auto const &transform{gameObject.GetComponent<roingine::Transform>()};

		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::BombDetonated>(
		        transform.GetWorldPosition()
		);

		gameObject.Destroy();
	}
}// namespace bomberman
