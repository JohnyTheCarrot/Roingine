#ifndef RECT_RENDERER_H
#define RECT_RENDERER_H

#include "component.h"

namespace roingine {
	class Transform;

	class RectRenderer final : public Component {
	public:
		RectRenderer(GameObject &gameObject, float width, float height);

		~RectRenderer() override = default;

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

	private:
		Transform                 &m_Transform;
		float                      m_Width, m_Height;
	};
}// namespace roingine

#endif// RECT_RENDERER_H
