#ifndef REUSABLE_TEXTURE_H
#define REUSABLE_TEXTURE_H

#include <glm/vec2.hpp>
#include <string_view>

#include <roingine/gl_texture.h>
#include <roingine/image_scaling.h>

namespace roingine {
	class ReusableTexture final {
		UniqueGLTexture m_TextureID{};
		int             m_ScalingMethod;
		float           m_UnitWidth{}, m_UnitHeight{};

	public:
		explicit ReusableTexture(std::string_view fileName, ScalingMethod scalingMethod = ScalingMethod::Linear);

		void Render(glm::vec2 pos, float width, float height) const noexcept;
	};
}// namespace roingine

#endif//REUSABLE_TEXTURE_H
