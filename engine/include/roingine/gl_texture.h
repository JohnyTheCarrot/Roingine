#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <cstdint>
#include <optional>

namespace roingine {
	class UniqueGLTexture final {
		std::optional<std::uint32_t> m_TextureID{};

	public:
		UniqueGLTexture();

		explicit UniqueGLTexture(std::uint32_t textureID);

		~UniqueGLTexture();

		UniqueGLTexture(UniqueGLTexture const &) = delete;

		UniqueGLTexture &operator=(UniqueGLTexture const &) = delete;

		UniqueGLTexture(UniqueGLTexture &&) noexcept;

		UniqueGLTexture &operator=(UniqueGLTexture &&) noexcept;

		[[nodiscard]]
		std::uint32_t Get() const;
	};
}// namespace roingine

#endif//GL_TEXTURE_H
