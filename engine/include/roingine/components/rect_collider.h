#ifndef RECT_COLLIDER_H
#define RECT_COLLIDER_H

#include <glm/glm.hpp>
#include <roingine/components/component.h>
#include <roingine/components/transform.h>

namespace roingine {
	class RectCollider final : public Component {
	public:
		enum class HitDirection {
			Top    = 0,
			Bottom = 1,
			Left   = 2,
			Right  = 3,
		};

		using Callback = std::function<void(GameObject, glm::vec2, HitDirection)>;

		RectCollider(GameObject &gameObject, float width, float height);

		void Update() override;

		void PostUpdate() override;

		void FixedUpdate() override;

		void Render() const override;

		void SetCallback(Callback const &callback);

		template<class T>
		[[nodiscard]]
		bool IsCollidingWith(T const &collider) const {
			return collider.IsCollidingWith(*this);
		}

		[[nodiscard]]
		bool IsCollidingWith(RectCollider const &collider) const;

		[[nodiscard]]
		Transform const &GetTransform() const;

		[[nodiscard]]
		float GetWidth() const noexcept;

		[[nodiscard]]
		float GetHeight() const noexcept;

	private:
		Transform              &m_Transform;
		float                   m_Width, m_Height;
		std::optional<Callback> m_Callback;
	};
}// namespace roingine

#endif// RECT_COLLIDER_H
