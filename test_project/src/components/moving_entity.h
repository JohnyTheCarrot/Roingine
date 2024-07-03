#ifndef MOVABLE_H
#define MOVABLE_H

#include <glm/vec2.hpp>
#include <roingine/components/component.h>

namespace roingine {
	class Transform;
}

namespace bomberman {
	class MovingEntity final : public roingine::Component {
		roingine::Transform *m_rpTransform;
		float                m_Speed;

	public:
		MovingEntity(roingine::GameObject &gameObject, float speed);

		void Move(glm::vec2 direction) const;
	};
}// namespace bomberman

#endif//MOVABLE_H
