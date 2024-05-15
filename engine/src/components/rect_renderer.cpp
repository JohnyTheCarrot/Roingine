#include <SDL_opengl.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>

namespace roingine {
	RectRenderer::RectRenderer(GameObject &gameObject, float width, float height)
	    : Component{gameObject}
	    , m_Transform{gameObject.GetComponent<Transform>()}
	    , m_Width{width}
	    , m_Height{height} {
	}

	void RectRenderer::Update() {
	}

	void RectRenderer::FixedUpdate() {
	}

	void RectRenderer::Render() const {
		TransformContext context{m_Transform};

		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_POLYGON);
		{
			glVertex2f(0.f, 0.f);
			glVertex2f(m_Width, 0.f);
			glVertex2f(m_Width, m_Height);
			glVertex2f(0.f, m_Height);
		}
		glEnd();
	}
}// namespace roingine
