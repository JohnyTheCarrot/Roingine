#include "unique_sdl_surface.h"
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <roingine/components/animation_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/game_time.h>

namespace roingine {
	AnimationRenderer::AnimationRenderer(
	        GameObject &gameObject, std::string const &fileName, std::size_t numFrames, float secondsPerFrame
	)
	    : Component{gameObject}
	    , m_Transform{gameObject.GetComponent<Transform>()}
	    , m_NumFrames{numFrames}
	    , m_SecondsPerFrame{secondsPerFrame} {
		UniqueSDLSurface sdlSurface{IMG_Load(fileName.c_str())};

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		m_Width  = static_cast<float>(sdlSurface->w);
		m_Height = static_cast<float>(sdlSurface->h);

		int const mode{sdlSurface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB};
		glTexImage2D(
		        GL_TEXTURE_2D, 0, mode, sdlSurface->w, sdlSurface->h, 0, mode, GL_UNSIGNED_BYTE, sdlSurface->pixels
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	AnimationRenderer::AnimationRenderer(
	        GameObject &gameObject, std::string const &fileName, std::size_t numFrames, float secondsPerFrame,
	        float width, float height
	)
	    : AnimationRenderer{gameObject, fileName, numFrames, secondsPerFrame} {
		m_Width  = width;
		m_Height = height;
	}

	void AnimationRenderer::Update() {
		auto const dt{GameTime::GetInstance().GetDeltaTime()};
		m_SecondsSinceFrame += dt;

		if (m_SecondsSinceFrame >= m_SecondsPerFrame) {
			++m_CurrentFrame;
			m_CurrentFrame %= m_NumFrames;
			m_SecondsSinceFrame = 0.f;
		}
	}

	void AnimationRenderer::FixedUpdate() {
	}

	void AnimationRenderer::Render() const {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		TransformContext context{m_Transform};

		float       X      = 0.f;
		float       Y      = 0.f;
		float       width  = m_Width;
		float       height = m_Height;
		float const u1{static_cast<float>(m_CurrentFrame) / static_cast<float>(m_NumFrames)};
		float const u2{static_cast<float>(m_CurrentFrame + 1) / static_cast<float>(m_NumFrames)};

		glBegin(GL_QUADS);
		{
			glTexCoord2f(u1, 1);
			glVertex3f(X, Y, 0);
			glTexCoord2f(u2, 1);
			glVertex3f(X + width, Y, 0);
			glTexCoord2f(u2, 0);
			glVertex3f(X + width, Y + height, 0);
			glTexCoord2f(u1, 0);
			glVertex3f(X, Y + height, 0);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	char const *AnimationRenderer::GetName() const {
		return NAME;
	}

	duk_function_list_entry const *AnimationRenderer::SetUpScriptAPI(duk_context *) const {
		return nullptr;
	}

	std::unique_ptr<AnimationRenderer>
	AnimationRenderer::JSFactory(GameObject *pGameObject, std::vector<JSData> const &args) {
		auto const fileName{comp_init::RequireString(0, args)};

		auto const numFrames{comp_init::RequireDouble(1, args)};
		auto const secondsPerFrame{comp_init::RequireDouble(2, args)};
		auto const width{comp_init::RequireDouble(3, args)};
		auto const height{comp_init::RequireDouble(4, args)};

		return std::make_unique<AnimationRenderer>(
		        *pGameObject, fileName, static_cast<std::size_t>(numFrames), static_cast<float>(secondsPerFrame),
		        static_cast<float>(width), static_cast<float>(height)
		);
	}
}// namespace roingine
