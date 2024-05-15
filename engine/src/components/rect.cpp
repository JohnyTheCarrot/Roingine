#include <roingine/components/rect.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>

namespace roingine {
	Rect::Rect(GameObject &gameObject, float width, float height)
	    : Component{gameObject}
	    , m_Width{width}
	    , m_Height{height} {
	}

	void Rect::Update() {
	}

	void Rect::FixedUpdate() {
	}

	void Rect::Render() const {
	}

	Corners Rect::GetCorners(Transform &transform) const noexcept {
		auto const worldPos{transform.GetWorldPosition()};
		auto const minX{worldPos.x};
		auto const maxX{worldPos.x + m_Width};
		auto const minY{worldPos.y};
		auto const maxY{worldPos.y + m_Height};

		return {.topLeft = worldPos, .bottomLeft = {minX, maxY}, .topRight = {maxX, minY}, .bottomRight = {maxX, maxY}};
	}
}// namespace roingine
