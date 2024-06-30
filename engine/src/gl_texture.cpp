#include "gl_texture.h"

namespace roingine {
	UniqueGLTexture::UniqueGLTexture() = default;

	UniqueGLTexture::UniqueGLTexture(GLuint textureID)
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

	GLuint UniqueGLTexture::Get() const {
		return m_TextureID.value();
	}
}// namespace roingine
