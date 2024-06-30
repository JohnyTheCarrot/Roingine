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
		UniqueSDLSurface const sdlSurface{IMG_Load(fileName.c_str())};

		GLuint textureId;
		glGenTextures(1, &textureId);
		m_TextureID = UniqueGLTexture{textureId};
		glBindTexture(GL_TEXTURE_2D, m_TextureID.Get());
		m_Width      = static_cast<float>(sdlSurface->w);
		m_Height     = static_cast<float>(sdlSurface->h);
		m_UnitWidth  = m_Width;
		m_UnitHeight = m_Height;

		UniqueSDLPixelFormat const target{SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32)};
		UniqueSDLSurface const     actualSurface{SDL_ConvertSurface(sdlSurface.get(), target.get(), 0)};

		constexpr int mode{GL_RGBA};
		glTexImage2D(
		        GL_TEXTURE_2D, 0, mode, actualSurface->w, actualSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		        actualSurface->pixels
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	TextureRenderer::TextureRenderer(
	        GameObject &gameObject, std::string const &fileName, float width, float height,
	        std::optional<float> unitWidth, std::optional<float> unitHeight
	)
	    : TextureRenderer{gameObject, fileName} {
		m_Width      = width;
		m_Height     = height;
		m_UnitWidth  = unitWidth.value_or(width);
		m_UnitHeight = unitHeight.value_or(height);
	}

	void TextureRenderer::Update() {
	}

	void TextureRenderer::FixedUpdate() {
	}

	void TextureRenderer::Render() const {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID.Get());
		TransformContext context{m_Transform};

		float const x      = 0.f;
		float const y      = 0.f;
		float const width  = m_Width;
		float const height = m_Height;
		float const u{width / m_UnitWidth};
		float const v{height / m_UnitHeight};

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, v);
			glVertex3f(x, y, 0);
			glTexCoord2f(u, v);
			glVertex3f(x + width, y, 0);
			glTexCoord2f(u, 0);
			glVertex3f(x + width, y + height, 0);
			glTexCoord2f(0, 0);
			glVertex3f(x, y + height, 0);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}// namespace roingine
