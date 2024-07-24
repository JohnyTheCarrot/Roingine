#ifndef TEXTURE_RENDERER_H
#define TEXTURE_RENDERER_H

#include "roingine/image_scaling.h"


#include <roingine/components/component.h>
#include <roingine/gl_texture.h>

struct SDL_Texture;

namespace roingine {
	class Transform;

	class TextureRenderer final : public Component {
	public:
		TextureRenderer(
		        GameObject &gameObject, std::string const &fileName, ScalingMethod scalingMethod = ScalingMethod::Linear
		);

		TextureRenderer(
		        GameObject &gameObject, std::string const &fileName, float width, float height,
		        ScalingMethod scalingMethod = ScalingMethod::Linear, std::optional<float> unitWidth = std::nullopt,
		        std::optional<float> unitHeight = std::nullopt
		);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

	private:
		Transform      &m_Transform;
		UniqueGLTexture m_TextureID{};
		float           m_Width, m_Height;
		float           m_UnitWidth, m_UnitHeight;
		int             m_ScalingMethod;
	};
}// namespace roingine

#endif// TEXTURE_RENDERER_H
