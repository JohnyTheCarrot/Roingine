#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace roingine {
	RectCollider::RectCollider(GameObject &gameObject, float width, float height)
	    : Component(gameObject)
	    , m_Transform{gameObject.GetComponent<Transform>()}
	    , m_Width{width}
	    , m_Height{height} {
	}

	void RectCollider::Update() {
	}

	[[nodiscard]]
	bool RectCollider::IsCollidingWith(RectCollider const &collider) const {
		auto const worldPos{m_Transform.GetWorldPosition()};
		auto const otherWorldPos{collider.m_Transform.GetWorldPosition()};

		auto const left{worldPos.x};
		auto const right{worldPos.x + m_Width};
		auto const top{worldPos.y + m_Height};
		auto const bottom{worldPos.y};

		auto const otherLeft{otherWorldPos.x};
		auto const otherRight{otherWorldPos.x + collider.m_Width};
		auto const otherTop{otherWorldPos.y + collider.m_Height};
		auto const otherBottom{otherWorldPos.y};

		if (right <= otherLeft || left >= otherRight)
			return false;

		if (bottom >= otherTop || top <= otherBottom)
			return false;

		return true;
	}

	Transform const &RectCollider::GetTransform() const {
		return m_Transform;
	}

	float RectCollider::GetWidth() const noexcept {
		return m_Width;
	}

	float RectCollider::GetHeight() const noexcept {
		return m_Height;
	}

	void RectCollider::PostUpdate() {
		if (!m_Callback.has_value())
			return;

		auto const worldPos{m_Transform.GetWorldPosition()};

		GetGameObject().GetScene()->ForEveryComponentOfType<RectCollider>([&](RectCollider &other) {
			if (&other == this || !other.GetGameObject().GetEnabled())
				return;

			auto const otherWorldPos{other.m_Transform.GetWorldPosition()};

			auto const left{worldPos.x};
			auto const right{worldPos.x + m_Width};
			auto const top{worldPos.y + m_Height};
			auto const bottom{worldPos.y};

			auto const otherLeft{otherWorldPos.x};
			auto const otherRight{otherWorldPos.x + other.m_Width};
			auto const otherTop{otherWorldPos.y + other.m_Height};
			auto const otherBottom{otherWorldPos.y};

			if (right <= otherLeft || left >= otherRight)
				return;

			if (bottom >= otherTop || top <= otherBottom)
				return;

			auto const intersectTop{top > otherTop && bottom < otherTop};
			auto const intersectBottom{bottom < otherBottom && top > otherBottom};
			auto const intersectLeft{left < otherLeft && right > otherLeft};
			auto const intersectRight{right > otherRight && left < otherRight};
			auto const intersectX{intersectLeft || intersectRight};

			auto const distanceTop{std::abs(top - otherTop)};
			auto const distanceBottom{std::abs(bottom - otherBottom)};
			auto const smallestXDistance{std::min(std::abs(left - otherLeft), std::abs(right - otherRight))};

			glm::vec2    hitPoint{worldPos};
			HitDirection hitDir{};

			if (intersectTop && (!intersectX || distanceTop > smallestXDistance)) {
				hitPoint.y = otherTop;
				hitDir     = HitDirection::Top;
			} else if (intersectBottom && (!intersectX || distanceBottom > smallestXDistance)) {
				hitPoint.y = otherBottom - m_Height;
				hitDir     = HitDirection::Bottom;
			} else if (intersectLeft) {
				hitPoint.x = otherLeft - m_Width;
				hitDir     = HitDirection::Left;
			} else if (intersectRight) {
				hitPoint.x = otherRight;
				hitDir     = HitDirection::Right;
			}

			m_Callback.value()(other.GetGameObject(), hitPoint, hitDir);
		});
	}

	void RectCollider::FixedUpdate() {
	}

	void RectCollider::Render() const {
	}

	void RectCollider::SetCallback(Callback const &callback) {
		m_Callback = callback;
	}
}// namespace roingine
