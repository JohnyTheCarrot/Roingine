#ifndef RECT_COLLIDER_H
#define RECT_COLLIDER_H

#include <glm/glm.hpp>
#include <roingine/components/component.h>

namespace roingine {
	class Transform;

	class RectCollider final : public Component {
	public:
		enum class HitDirection {
			Top    = 0,
			Bottom = 1,
			Left   = 2,
			Right  = 3,
		};

		RectCollider(GameObject &gameObject, float width, float height);

		void Update() override;

		void PostUpdate() override;

		void FixedUpdate() override;

		void Render() const override;

		[[nodiscard]]
		bool GetHasListener() const noexcept;

		void SetHasListener(bool hasListener) noexcept;

	private:
		void CallJSCallback(GameObject otherGameObject, glm::vec2 hitPoint, HitDirection hitDirection);

		Transform &m_Transform;
		float      m_Width, m_Height;
		bool       m_HasListener{false};
	};
}// namespace roingine

#endif// RECT_COLLIDER_H
