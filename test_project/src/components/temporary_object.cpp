#include "temporary_object.h"

#include "../event_queue.h"

#include <roingine/components/transform.h>
#include <roingine/game_time.h>

#include <utility>

namespace bomberman {
	TemporaryObject::TemporaryObject(roingine::GameObject gameObject, float maxAge, std::optional<OnDeathFn> onDeath)
	    : Component{gameObject}
	    , m_OnDeath{std::move(onDeath)}
	    , m_MaxAge{maxAge} {
	}

	void TemporaryObject::Update() {
		m_Age += roingine::GameTime::GetInstance().GetDeltaTime();

		if (m_Age < m_MaxAge)
			return;

		auto gameObject{GetGameObject()};

		if (m_OnDeath.has_value()) {
			(*m_OnDeath)(GetGameObject());
		}

		gameObject.Destroy();
	}
}// namespace bomberman
