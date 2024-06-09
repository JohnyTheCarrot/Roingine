#include "sdl_smart_pointers.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <roingine/component_init_types.h>
#include <roingine/components/texture_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>

namespace roingine {
	TextureRenderer::TextureRenderer(GameObject &gameObject, std::string const &fileName)
	    : Component{gameObject}
	    , m_Transform{gameObject.GetComponent<Transform>()} {
		UniqueSDLSurface sdlSurface{IMG_Load(fileName.c_str())};

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		m_Width  = static_cast<float>(sdlSurface->w);
		m_Height = static_cast<float>(sdlSurface->h);

		UniqueSDLPixelFormat target{SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32)};
		UniqueSDLSurface     actualSurface{SDL_ConvertSurface(sdlSurface.get(), target.get(), 0)};

		constexpr int mode{GL_RGBA};
		glTexImage2D(
		        GL_TEXTURE_2D, 0, mode, actualSurface->w, actualSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		        actualSurface->pixels
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	TextureRenderer::TextureRenderer(GameObject &gameObject, std::string const &fileName, float width, float height)
	    : roingine::TextureRenderer{gameObject, fileName, width, height, width, height} {
	}

	TextureRenderer::TextureRenderer(
	        GameObject &gameObject, std::string const &fileName, float width, float height, float unitWidth,
	        float unitHeight
	)
	    : roingine::TextureRenderer{gameObject, fileName} {
		m_Width      = width;
		m_Height     = height;
		m_UnitWidth  = unitWidth;
		m_UnitHeight = unitHeight;
	}

	void TextureRenderer::Update() {
	}

	void TextureRenderer::FixedUpdate() {
	}

	void TextureRenderer::Render() const {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		TransformContext context{m_Transform};

		float X      = 0.f;
		float Y      = 0.f;
		float width  = m_Width;
		float height = m_Height;
		float const u{width / m_UnitWidth};
		float const v{height / m_UnitHeight};

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, v);
			glVertex3f(X, Y, 0);
			glTexCoord2f(u, v);
			glVertex3f(X + width, Y, 0);
			glTexCoord2f(u, 0);
			glVertex3f(X + width, Y + height, 0);
			glTexCoord2f(0, 0);
			glVertex3f(X, Y + height, 0);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	char const *TextureRenderer::GetName() const {
		return NAME;
	}

	duk_function_list_entry const *TextureRenderer::SetUpScriptAPI(duk_context *) const {
		return nullptr;
	}

	std::unique_ptr<TextureRenderer>
	TextureRenderer::JSFactory(GameObject *pGameObject, std::vector<JSData> const &args) {
		auto const fileName{comp_init::RequireString(0, args)};

		switch (args.size()) {
			case 3: {
				auto const width{comp_init::RequireDouble(1, args)};
				auto const height{comp_init::RequireDouble(2, args)};

				return std::make_unique<TextureRenderer>(
				        *pGameObject, fileName, static_cast<float>(width), static_cast<float>(height)
				);
			}
			case 5: {
				auto const width{comp_init::RequireDouble(1, args)};
				auto const height{comp_init::RequireDouble(2, args)};
				auto const unitWidth{comp_init::RequireDouble(3, args)};
				auto const unitHeight{comp_init::RequireDouble(4, args)};

				return std::make_unique<TextureRenderer>(
				        *pGameObject, fileName, static_cast<float>(width), static_cast<float>(height),
				        static_cast<float>(unitWidth), static_cast<float>(unitHeight)
				);
			}
			default:
				return std::make_unique<TextureRenderer>(*pGameObject, fileName);
		}
	}
}// namespace roingine
