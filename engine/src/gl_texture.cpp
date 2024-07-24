#include <SDL_opengl.h>
#include <roingine/gl_texture.h>

namespace roingine {
	UniqueGLTexture::UniqueGLTexture() = default;

	UniqueGLTexture::UniqueGLTexture(std::uint32_t textureID)
	    : m_TextureID{textureID} {
	}

	UniqueGLTexture::~UniqueGLTexture() {
		if (m_TextureID.has_value())
			glDeleteTextures(1, &m_TextureID.value());
	}

	UniqueGLTexture::UniqueGLTexture(UniqueGLTexture &&other) noexcept {
		m_TextureID = other.m_TextureID;
		other.m_TextureID.reset();
	}

	UniqueGLTexture &UniqueGLTexture::operator=(UniqueGLTexture &&other) noexcept {
		if (this != &other) {
			m_TextureID = other.m_TextureID;
			other.m_TextureID.reset();
		}
		return *this;
	}

	std::uint32_t UniqueGLTexture::Get() const {
		return m_TextureID.value();
	}
}// namespace roingine
