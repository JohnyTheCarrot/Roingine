#ifndef RECT_H
#define RECT_H

#include <glm/glm.hpp>
#include <roingine/components/component.h>

namespace roingine {
	class Transform;

	struct Corners final {
		glm::vec2 topLeft;
		glm::vec2 bottomLeft;
		glm::vec2 topRight;
		glm::vec2 bottomRight;
	};

	struct Rect final : public Component {
		Rect(GameObject &gameObject, float width, float height);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		[[nodiscard]]
		Corners GetCorners(Transform &transform) const noexcept;

		float m_Width, m_Height;
	};
}// namespace roingine

#endif// RECT_H
