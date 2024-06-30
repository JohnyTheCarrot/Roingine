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

		float const r{m_ColorR / 255.f};
		float const g{m_ColorG / 255.f};
		float const b{m_ColorB / 255.f};

		glBegin(GL_POLYGON);
		{
			glColor3f(r, g, b);

			glVertex2f(0.f, 0.f);
			glVertex2f(m_Width, 0.f);
			glVertex2f(m_Width, m_Height);
			glVertex2f(0.f, m_Height);

			glColor3f(1.f, 1.f, 1.f);
		}
		glEnd();
	}

	void RectRenderer::SetColor(uint8_t r, uint8_t g, uint8_t b) {
		m_ColorR = r;
		m_ColorG = g;
		m_ColorB = b;
	}
}// namespace roingine
