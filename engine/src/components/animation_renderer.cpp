#include "sdl_smart_pointers.h"
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <duktape.h>
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

		UniqueSDLPixelFormat target{SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32)};
		UniqueSDLSurface     actualSurface{SDL_ConvertSurface(sdlSurface.get(), target.get(), 0)};

		constexpr int mode{GL_RGBA};
		glTexImage2D(
		        GL_TEXTURE_2D, 0, mode, actualSurface->w, actualSurface->h, 0, mode, GL_UNSIGNED_BYTE,
		        actualSurface->pixels
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
		if (m_Flipped) {
			glTranslatef(m_Width, 0.f, 0.f);
			glRotatef(180.f, 0.f, 1.f, 0.f);
		}

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

	int JsAPIGetFlipped(duk_context *ctx) {
		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		AnimationRenderer *ptr{static_cast<AnimationRenderer *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		auto const flipped{ptr->GetFlipped()};
		duk_push_boolean(ctx, flipped);

		return 1;
	}

	int JsAPISetFlipped(duk_context *ctx) {
		auto const isFlipped{duk_require_boolean(ctx, 0)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		AnimationRenderer *ptr{static_cast<AnimationRenderer *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		ptr->SetFlipped(isFlipped);

		return 0;
	}

	duk_function_list_entry const rendererAPI[]{
	        {"getFlipped", JsAPIGetFlipped, 0},
	        {"setFlipped", JsAPISetFlipped, 1},
	        {nullptr, nullptr, 0}
	};

	duk_function_list_entry const *AnimationRenderer::SetUpScriptAPI(duk_context *) const {
		return rendererAPI;
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

	bool AnimationRenderer::GetFlipped() const {
		return m_Flipped;
	}

	void AnimationRenderer::SetFlipped(bool flipped) {
		m_Flipped = flipped;
	}
}// namespace roingine
