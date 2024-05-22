#include <SDL_opengl.h>
#include <roingine/components/rect.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>

namespace roingine {
	RectRenderer::RectRenderer(GameObject &gameObject)
	    : Component{gameObject}
	    , m_Transform{gameObject.GetComponent<Transform>()}
	    , m_Rect{gameObject.GetComponent<Rect>()} {
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
			glVertex2f(m_Rect.m_Width, 0.f);
			glVertex2f(m_Rect.m_Width, m_Rect.m_Height);
			glVertex2f(0.f, m_Rect.m_Height);
		}
		glEnd();
	}

	char const *RectRenderer::GetName() const {
		return NAME;
	}

	duk_function_list_entry const *RectRenderer::SetUpScriptAPI(duk_context *) const {
		return nullptr;
	}

	std::unique_ptr<RectRenderer> RectRenderer::JSFactory(GameObject *pGameObject, duk_context *) {
		return std::make_unique<RectRenderer>(*pGameObject);
	}
}// namespace roingine
