#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <roingine/component_init_types.h>
#include <roingine/components/texture_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>

namespace roingine {
	class SDLSurfaceDestroyer final {
	public:
		void operator()(SDL_Surface *surface) {
			SDL_FreeSurface(surface);
		}
	};

	using UniqueSDLSurface = std::unique_ptr<SDL_Surface, SDLSurfaceDestroyer>;

	TextureRenderer::TextureRenderer(GameObject &gameObject, std::string const &fileName)
	    : Component{gameObject}
	    , m_Transform{gameObject.GetComponent<Transform>()} {
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

	TextureRenderer::TextureRenderer(GameObject &gameObject, std::string const &fileName, float width, float height)
	    : roingine::TextureRenderer{gameObject, fileName} {
		m_Width  = width;
		m_Height = height;
	}

	void TextureRenderer::Update() {
	}

	void TextureRenderer::FixedUpdate() {
	}

	void TextureRenderer::Render() const {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		TransformContext context{m_Transform};

		float X      = 0.f;
		float Y      = 0.f;
		float width  = m_Width;
		float height = m_Height;

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1);
			glVertex3f(X, Y, 0);
			glTexCoord2f(1, 1);
			glVertex3f(X + width, Y, 0);
			glTexCoord2f(1, 0);
			glVertex3f(X + width, Y + height, 0);
			glTexCoord2f(0, 0);
			glVertex3f(X, Y + height, 0);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	char const *TextureRenderer::GetName() const {
		return NAME;
	}

	duk_function_list_entry const *TextureRenderer::SetUpScriptAPI(duk_context *) const {
		return nullptr;
	}

	std::unique_ptr<TextureRenderer>
	TextureRenderer::JSFactory(GameObject *pGameObject, std::vector<JSData> const &args) {
		auto const fileName{comp_init::RequireString(0, args)};
		if (args.size() == 3) {
			auto const width{comp_init::RequireDouble(1, args)};
			auto const height{comp_init::RequireDouble(2, args)};

			return std::make_unique<TextureRenderer>(
			        *pGameObject, fileName, static_cast<float>(width), static_cast<float>(height)
			);
		}

		return std::make_unique<TextureRenderer>(*pGameObject, fileName);
	}
}// namespace roingine
