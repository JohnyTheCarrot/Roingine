#ifndef RECT_RENDERER_H
#define RECT_RENDERER_H

#include "component.h"
#include <cstdint>

namespace roingine {
	struct Rect;

	class Transform;

	class RectRenderer final : public Component {
	public:
		RectRenderer(GameObject &gameObject, float width, float height);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		void SetColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);

	private:
		Transform   &m_Transform;
		float        m_Width, m_Height;
		std::uint8_t m_ColorR{0xFF}, m_ColorG{0xFF}, m_ColorB{0xFF};
	};
}// namespace roingine

#endif// RECT_RENDERER_H
