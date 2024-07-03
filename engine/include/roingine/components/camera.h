#ifndef CAMERA_H
#define CAMERA_H

#include <array>
#include <roingine/components/component.h>

namespace roingine {
	class Transform;

	class Camera final : public Component {
		Transform *m_pTransform;
		int        m_ViewX, m_ViewY, m_ViewWidth, m_ViewHeight;

	public:
		Camera(GameObject &gameObject, int viewX, int viewY, int viewWidth, int viewHeight);

		void Render() const override;

		void RenderScene() const;
	};
}// namespace roingine

#endif//CAMERA_H
