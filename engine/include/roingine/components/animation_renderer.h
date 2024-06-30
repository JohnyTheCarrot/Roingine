#ifndef ANIMATION_RENDERER_H
#define ANIMATION_RENDERER_H

#include <roingine/components/component.h>

struct SDL_Texture;

namespace roingine {
	class Transform;

	class AnimationRenderer final : public Component {
	public:
		AnimationRenderer(
		        GameObject &gameObject, std::string const &fileName, std::size_t numFrames, float secondsPerFrame
		);

		AnimationRenderer(
		        GameObject &gameObject, std::string const &fileName, std::size_t numFrames, float secondsPerFrame,
		        float width, float height
		);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		[[nodiscard]]
		bool GetFlipped() const;

		void SetFlipped(bool flipped);

	private:
		Transform    &m_Transform;
		std::uint32_t m_TextureID;
		std::size_t   m_NumFrames;
		float         m_Width, m_Height;
		float         m_SecondsPerFrame;
		float         m_SecondsSinceFrame{0.f};
		int           m_CurrentFrame{0};
		bool          m_Flipped{false};
	};
}// namespace roingine

#endif// ANIMATION_RENDERER_H
