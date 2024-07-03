#include "moving_entity.h"
#include <roingine/components/transform.h>
#include <roingine/game_time.h>

namespace bomberman {
	MovingEntity::MovingEntity(roingine::GameObject &gameObject, float speed)
	    : roingine::Component{gameObject}
	    , m_rpTransform{&gameObject.GetComponent<roingine::Transform>()}
	    , m_Speed{speed} {
	}

	void MovingEntity::Move(glm::vec2 direction) const {
		direction *= m_Speed * roingine::GameTime::GetInstance().GetDeltaTime();

		m_rpTransform->Translate(direction);
	}
}// namespace bomberman
