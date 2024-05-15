#include <SDL_opengl.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numbers>
#include <roingine/components/transform.h>
#include <roingine/game_time.h>

namespace roingine {
	Transform::Transform(GameObject &gameObject, glm::vec2 position, float rotation)
	    : Component{gameObject}
	    , m_Position{position}
	    , m_Rotation{rotation} {
	}

	void Transform::Update() {
	}

	void Transform::FixedUpdate() {
	}

	void Transform::Render() const {
	}

	void Transform::Rotate(float angle) noexcept {
		constexpr float MAX_ANGLE{360.0f};

		auto const rad{angle * (std::numbers::pi_v<float> / 180.f)};
		m_Rotation += rad * GameTime::GetInstance().GetDeltaTime();

		m_Rotation = m_Rotation - std::floor(m_Rotation / MAX_ANGLE) * MAX_ANGLE;
	}

	void Transform::Translate(glm::vec2 translation) noexcept {
		m_Position += translation;
	}

	void Transform::Translate(float x, float y) noexcept {
		m_Position.x += x;
		m_Position.y += y;
	}

	glm::vec2 Transform::GetLocalPosition() const noexcept {
		return m_Position;
	}

	glm::vec2 Transform::GetWorldPosition() const noexcept {
		auto const transformationMat{GetTransformationMatrix()};

		return glm::vec4{m_Position, 0.f, 1.f} * transformationMat;
	}

	float Transform::GetRotation() const noexcept {
		return m_Rotation;
	}

	glm::vec2 Transform::GetPivot() const noexcept {
		return m_Pivot;
	}

	void Transform::SetPivot(glm::vec2 pivot) noexcept {
		m_Pivot = pivot;
	}

	void Transform::SetPivot(float x, float y) noexcept {
		m_Pivot = {x, y};
	}

	void Transform::SetParent(Transform &parent) {
		m_pParent = &parent;
	}

	Transform &Transform::GetParent() const noexcept {
		return *m_pParent;
	}

	glm::mat4 Transform::GetTransformationMatrix() const noexcept {
		glm::mat4        transMat{1.f};
		Transform const *pCurrentTransform{this};

		do {
			glm::mat4        currentTransMat;
			glm::vec2 const &pivot{pCurrentTransform->GetPivot()};
			currentTransMat = glm::translate(glm::mat4{1.f}, glm::vec3{pCurrentTransform->GetLocalPosition(), 0.f});

			if (pivot != glm::vec2{0, 0})
				currentTransMat = glm::translate(currentTransMat, glm::vec3{pivot, 0.f});

			currentTransMat = glm::rotate(currentTransMat, GetRotation(), glm::vec3{0, 0, 1});

			if (pivot != glm::vec2{0, 0})
				currentTransMat = glm::translate(currentTransMat, glm::vec3{-pivot, 0.f});

			transMat = currentTransMat * transMat;
		} while ((pCurrentTransform = &pCurrentTransform->GetParent()) != nullptr);

		return transMat;
	}

	TransformContext::TransformContext(Transform const &transform) noexcept {
		auto const transformationMat{transform.GetTransformationMatrix()};

		glLoadIdentity();
		glMultMatrixf(glm::value_ptr(transformationMat));
	}

	TransformContext::~TransformContext() noexcept {
		glPopMatrix();
	}
}// namespace roingine
