#include "sdl_smart_pointers.h"

#include <SDL_image.h>
#include <SDL_opengl.h>
#include <roingine/reusable_texture.h>

namespace roingine {
	ReusableTexture::ReusableTexture(std::string_view fileName, ScalingMethod scalingMethod)
	    : m_ScalingMethod{scaling_method::ToOpenGLScalingMethod(scalingMethod)} {
		UniqueSDLSurface const sdlSurface{IMG_Load(fileName.data())};

		GLuint textureId;
		glGenTextures(1, &textureId);
		m_TextureID = UniqueGLTexture{textureId};
		glBindTexture(GL_TEXTURE_2D, m_TextureID.Get());

		m_UnitWidth  = static_cast<float>(sdlSurface->w);
		m_UnitHeight = static_cast<float>(sdlSurface->h);

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

	void ReusableTexture::Render(glm::vec2 pos, float width, float height) const noexcept {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID.Get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_ScalingMethod);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_ScalingMethod);

		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.f);

		float const x = 0.f;
		float const y = 0.f;
		float const u{1.f};
		float const v{1.f};

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

		glPopMatrix();
	}
}// namespace roingine
