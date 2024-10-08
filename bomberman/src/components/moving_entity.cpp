#include "moving_entity.h"

#include "level_flyweight.h"
#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>
#include <roingine/game_time.h>

namespace bomberman {
	MovingEntity::MovingEntity(roingine::GameObject &gameObject, LevelFlyweight const &levelFlyweight, float speed)
	    : Component{gameObject}
	    , m_rpTransform{&gameObject.GetComponent<roingine::Transform>()}
	    , m_rpRectCollider{&gameObject.GetComponent<roingine::RectCollider>()}
	    , m_rpLevelFlyweight{&levelFlyweight}
	    , m_Speed{speed} {
	}

	bool MovingEntity::Move(glm::vec2 direction) const {
		direction *= m_Speed * roingine::GameTime::GetInstance().GetDeltaTime();

		if (auto const colPoint{m_rpLevelFlyweight->GetCollisionPoint(
		            m_rpTransform->GetWorldPosition() + direction, m_rpRectCollider->GetWidth(),
		            m_rpRectCollider->GetHeight()
		    )};
		    colPoint.has_value()) {
			m_rpTransform->SetLocalPosition(colPoint.value());
			return false;
		}

		m_rpTransform->Translate(direction);
		return true;
	}

	LevelFlyweight const &MovingEntity::GetLevelFlyweight() const {
		return *m_rpLevelFlyweight;
	}
}// namespace bomberman
