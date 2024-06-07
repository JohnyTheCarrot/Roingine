#include <SDL_opengl.h>
#include <duktape.h>
#include <iostream>
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

	char const *RectRenderer::GetName() const {
		return NAME;
	}

	int JsAPISetColor(duk_context *ctx) {
		auto const numArgs{duk_get_top(ctx)};

		std::uint8_t r, g, b;

		switch (numArgs) {
			case 1: {
				auto const color{duk_require_int(ctx, 0)};
				r = static_cast<std::uint8_t>((color >> 16) & 0xFF);
				g = static_cast<std::uint8_t>((color >> 8) & 0xFF);
				b = static_cast<std::uint8_t>(color & 0xFF);

				break;
			}
			case 3: {
				r = static_cast<std::uint8_t>(duk_require_int(ctx, 0));
				g = static_cast<std::uint8_t>(duk_require_int(ctx, 1));
				b = static_cast<std::uint8_t>(duk_require_int(ctx, 2));

				break;
			}
			default:
				std::cout << "setColor must take either 1 or 3 arguments" << std::endl;
				return DUK_RET_ERROR;
		}

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		RectRenderer *ptr{static_cast<RectRenderer *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		ptr->SetColor(r, g, b);

		return 0;
	}

	duk_function_list_entry const rectRendererAPI[]{{"setColor", JsAPISetColor, DUK_VARARGS}, {nullptr, nullptr, 0}};

	duk_function_list_entry const *RectRenderer::SetUpScriptAPI(duk_context *) const {
		return rectRendererAPI;
	}

	std::unique_ptr<RectRenderer> RectRenderer::JSFactory(GameObject *pGameObject, std::vector<JSData> const &args) {
		auto const width{comp_init::RequireDouble(0, args)};
		auto const height{comp_init::RequireDouble(1, args)};

		return std::make_unique<RectRenderer>(*pGameObject, static_cast<float>(width), static_cast<float>(height));
	}

	void RectRenderer::SetColor(uint8_t r, uint8_t g, uint8_t b) {
		m_ColorR = r;
		m_ColorG = g;
		m_ColorB = b;
	}
}// namespace roingine
