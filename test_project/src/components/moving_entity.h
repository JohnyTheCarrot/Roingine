#ifndef MOVABLE_H
#define MOVABLE_H

#include <glm/vec2.hpp>
#include <roingine/components/component.h>

namespace bomberman {
	class LevelFlyweight;
}

namespace roingine {
	class RectCollider;
	class Transform;
}// namespace roingine

namespace bomberman {
	class MovingEntity final : public roingine::Component {
		roingine::Transform    *m_rpTransform;
		roingine::RectCollider *m_rpRectCollider;
		LevelFlyweight const   *m_rpLevelFlyweight;
		float                   m_Speed;

	public:
		MovingEntity(roingine::GameObject &gameObject, LevelFlyweight const &levelFlyweight, float speed);

		void Move(glm::vec2 direction) const;
	};
}// namespace bomberman

#endif//MOVABLE_H
