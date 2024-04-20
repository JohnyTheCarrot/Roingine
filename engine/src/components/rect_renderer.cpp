#include "components/rect_renderer.h"
#include "components/transform.h"
#include "gameobject.h"
#include <SDL_opengl.h>

namespace roingine {
	RectRenderer::RectRenderer(gsl::not_null<GameObject *> pGameObject, int width, int height)
	    : Component{pGameObject}
	    , m_pTransform{pGameObject->GetComponent<Transform>()}
	    , m_Width{width}
	    , m_Height{height} {
	}

	void RectRenderer::Update() {
	}

	void RectRenderer::FixedUpdate() {
	}

	void RectRenderer::Render() const {
		TransformContext context{*m_pTransform};

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_POLYGON);
		{
			glVertex2f(0.f, 0.f);
			glVertex2f(100.f, 0.f);
			glVertex2f(100.f, 100.f);
			glVertex2f(0.f, 100.f);
		}
		glEnd();
	}
}// namespace roingine
