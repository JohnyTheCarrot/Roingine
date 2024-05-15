#ifndef RECT_RENDERER_H
#define RECT_RENDERER_H

#include "component.h"

namespace roingine {
	struct Rect;

	class Transform;

	class RectRenderer final : public Component {
	public:
		RectRenderer(GameObject &gameObject);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

	private:
		Transform &m_Transform;
		Rect      &m_Rect;
	};
}// namespace roingine

#endif// RECT_RENDERER_H
