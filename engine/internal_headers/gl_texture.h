#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <SDL_opengl.h>
#include <optional>

namespace roingine {
	class UniqueGLTexture final {
		std::optional<GLuint> m_TextureID{};

	public:
		UniqueGLTexture();

		explicit UniqueGLTexture(GLuint textureID);

		~UniqueGLTexture();

		UniqueGLTexture(UniqueGLTexture const &) = delete;

		UniqueGLTexture &operator=(UniqueGLTexture const &) = delete;

		UniqueGLTexture(UniqueGLTexture &&) noexcept;

		UniqueGLTexture &operator=(UniqueGLTexture &&) noexcept;

		[[nodiscard]]
		GLuint Get() const;
	};
}// namespace roingine

#endif//GL_TEXTURE_H
