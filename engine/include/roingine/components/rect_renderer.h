#ifndef RECT_RENDERER_H
#define RECT_RENDERER_H

#include "component.h"
#include <gsl/pointers>

namespace roingine {
	class Transform;

	class RectRenderer final : public Component {
	public:
		RectRenderer(gsl::not_null<GameObject *> pGameObject, int width, int height);

		~RectRenderer() override = default;

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

	private:
		gsl::not_null<Transform *> m_pTransform;
		int                        m_Width, m_Height;
	};
}// namespace roingine

#endif// RECT_RENDERER_H
