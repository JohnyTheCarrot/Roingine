#include <SDL_opengl.h>
#include <roingine/components/transform.h>

namespace roingine {
	Transform::Transform(gsl::not_null<GameObject *> pGameObject, math::Vec2 position, math::FloatingPoint rotation)
	    : Component{pGameObject}
	    , m_Position{position}
	    , m_Rotation{rotation} {
	}

	void Transform::Update() {
	}

	void Transform::FixedUpdate() {
	}

	void Transform::Render() const {
	}

	math::Vec2 Transform::GetLocalPosition() {
		return m_Position;
	}

	math::FloatingPoint Transform::GetRotation() {
		return m_Rotation;
	}

	TransformContext::TransformContext(Transform &transform) {
		using namespace math;
		glPushMatrix();
		math::Vec2 const   &pos{transform.GetLocalPosition()};
		math::FloatingPoint rotAngle{transform.GetRotation()};

		glTranslate(pos.x, pos.y, 0.0_fp);
		glRotate(rotAngle, 0.0_fp, 0.0_fp, 1.0_fp);
	}

	TransformContext::~TransformContext() {
		glPopMatrix();
	}
}// namespace roingine
