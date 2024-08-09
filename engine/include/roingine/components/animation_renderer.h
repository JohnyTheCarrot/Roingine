#ifndef ANIMATION_RENDERER_H
#define ANIMATION_RENDERER_H

#include <roingine/components/component.h>
#include <roingine/gl_texture.h>
#include <roingine/image_scaling.h>

struct SDL_Texture;

namespace roingine {
	class Transform;

	class AnimationRenderer final : public Component {
	public:
		AnimationRenderer(
		        GameObject &gameObject, std::string const &fileName, int numFrames, float secondsPerFrame,
		        std::optional<float> width = std::nullopt, std::optional<float> height = std::nullopt,
		        ScalingMethod scalingMethod = ScalingMethod::Linear
		);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		[[nodiscard]]
		bool GetFlipped() const;

		void SetFlipped(bool flipped);

		void SetFrameRange(int startFrame, int endFrame);

		[[nodiscard]]
		bool IsPaused() const noexcept;

		void SetPaused(bool isPaused) noexcept;

	private:
		Transform      &m_Transform;
		UniqueGLTexture m_TextureID{};
		int             m_ScalingMethod;
		int             m_StartFrame{0};
		int             m_EndFrame;
		int             m_NumFrames;
		int             m_CurrentFrame{0};
		float           m_Width, m_Height;
		float           m_SecondsPerFrame;
		float           m_SecondsSinceFrame{0.f};
		bool            m_Flipped{false};
		bool            m_IsPaused{false};
	};
}// namespace roingine

#endif// ANIMATION_RENDERER_H
