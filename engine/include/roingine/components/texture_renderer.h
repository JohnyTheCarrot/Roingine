#ifndef TEXTURE_RENDERER_H
#define TEXTURE_RENDERER_H

#include <roingine/components/component.h>

struct SDL_Texture;

namespace roingine {
	class Transform;

	class TextureRenderer final : public Component {
	public:
		TextureRenderer(GameObject &gameObject, std::string const &fileName);

		TextureRenderer(GameObject &gameObject, std::string const &fileName, float width, float height);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		static constexpr char const *NAME{"TextureRenderer"};

		[[nodiscard]]
		char const *GetName() const override;

		[[nodiscard]]
		duk_function_list_entry const *SetUpScriptAPI(duk_context *) const override;

		[[nodiscard]]
		static std::unique_ptr<TextureRenderer> JSFactory(GameObject *, std::vector<JSData> const &args);

	private:
		Transform    &m_Transform;
		std::uint32_t m_TextureID;
		float         m_Width, m_Height;
	};
}// namespace roingine

#endif// TEXTURE_RENDERER_H
