// clang-format off
#include <windows.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
// clang-format on
#include <roingine/components/camera.h>
#include <roingine/components/transform.h>
#include <roingine/scene.h>

namespace roingine {
	Camera::Camera(GameObject &gameObject, int viewX, int viewY, int viewWidth, int viewHeight)
	    : Component{gameObject}
	    , m_pTransform{&gameObject.GetComponent<Transform>()}
	    , m_ViewX{viewX}
	    , m_ViewY{viewY}
	    , m_ViewWidth{viewWidth}
	    , m_ViewHeight{viewHeight} {
	}

	void Camera::Render() const {
	}

	void Camera::RenderScene() const {
		glViewport(m_ViewX, m_ViewY, m_ViewWidth, m_ViewHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, m_ViewWidth, 0, m_ViewHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_SCISSOR_TEST);
		glScissor(m_ViewX, m_ViewY, m_ViewWidth, m_ViewHeight);
		glDisable(GL_SCISSOR_TEST);

		auto const position{m_pTransform->GetWorldPosition()};
		glPushMatrix();
		glTranslatef(
		        -position.x + static_cast<float>(m_ViewWidth) / 2.f,
		        -position.y + static_cast<float>(m_ViewHeight) / 2.f, 0.f
		);
		GetGameObject().GetScene()->Render();
		glPopMatrix();
	}
}// namespace roingine
