#include "sdl_smart_pointers.h"
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <roingine/components/animation_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/game_time.h>

namespace roingine {
	AnimationRenderer::AnimationRenderer(
	        GameObject &gameObject, std::string const &fileName, int numFrames, float secondsPerFrame,
	        std::optional<float> width, std::optional<float> height, ScalingMethod scalingMethod
	)
	    : Component{gameObject}
	    , m_Transform{gameObject.GetComponent<Transform>()}
	    , m_ScalingMethod{scaling_method::ToOpenGLScalingMethod(scalingMethod)}
	    , m_EndFrame{numFrames}
	    , m_NumFrames{numFrames}
	    , m_SecondsPerFrame{secondsPerFrame} {
		UniqueSDLSurface const sdlSurface{IMG_Load(fileName.c_str())};

		GLuint textureId;
		glGenTextures(1, &textureId);
		m_TextureID = UniqueGLTexture{textureId};
		glBindTexture(GL_TEXTURE_2D, m_TextureID.Get());

		m_Width  = width.value_or(static_cast<float>(sdlSurface->w));
		m_Height = height.value_or(static_cast<float>(sdlSurface->h));

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

	void AnimationRenderer::Update() {
		auto const dt{GameTime::GetInstance().GetDeltaTime()};
		m_SecondsSinceFrame += dt;

		if (m_SecondsSinceFrame >= m_SecondsPerFrame && !m_IsPaused) {
			++m_CurrentFrame;
			if (m_CurrentFrame >= m_EndFrame)
				m_CurrentFrame = m_StartFrame;
			m_SecondsSinceFrame = 0.f;
		}
	}

	void AnimationRenderer::FixedUpdate() {
	}

	void AnimationRenderer::Render() const {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID.Get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_ScalingMethod);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_ScalingMethod);
		TransformContext context{m_Transform};
		if (m_Flipped) {
			glTranslatef(m_Width, 0.f, 0.f);
			glRotatef(180.f, 0.f, 1.f, 0.f);
		}

		float const x      = 0.f;
		float const y      = 0.f;
		float const width  = m_Width;
		float const height = m_Height;
		float const u1{static_cast<float>(m_CurrentFrame) / static_cast<float>(m_NumFrames)};
		float const u2{static_cast<float>(m_CurrentFrame + 1) / static_cast<float>(m_NumFrames)};

		glBegin(GL_QUADS);
		{
			glTexCoord2f(u1, 1);
			glVertex3f(x, y, 0);
			glTexCoord2f(u2, 1);
			glVertex3f(x + width, y, 0);
			glTexCoord2f(u2, 0);
			glVertex3f(x + width, y + height, 0);
			glTexCoord2f(u1, 0);
			glVertex3f(x, y + height, 0);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	bool AnimationRenderer::GetFlipped() const {
		return m_Flipped;
	}

	void AnimationRenderer::SetFlipped(bool flipped) {
		m_Flipped = flipped;
	}

	void AnimationRenderer::SetFrameRange(int startFrame, int endFrame) {
		if (m_StartFrame != startFrame)
			m_CurrentFrame = startFrame;

		m_StartFrame = startFrame;
		m_EndFrame   = endFrame;
	}

	bool AnimationRenderer::IsPaused() const noexcept {
		return m_IsPaused;
	}

	void AnimationRenderer::SetPaused(bool isPaused) noexcept {
		m_IsPaused = isPaused;
	}
}// namespace roingine
