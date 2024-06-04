#ifndef RECT_RENDERER_H
#define RECT_RENDERER_H

#include "component.h"

namespace roingine {
	struct Rect;

	class Transform;

	class RectRenderer final : public Component {
	public:
		RectRenderer(GameObject &gameObject);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		static constexpr char const *NAME{"RectRenderer"};

		[[nodiscard]]
		char const *GetName() const override;

		[[nodiscard]]
		duk_function_list_entry const *SetUpScriptAPI(duk_context *) const override;

		[[nodiscard]]
		static std::unique_ptr<RectRenderer> JSFactory(GameObject *, const std::vector<JSData> &args);

		void SetColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);

	private:
		Transform &m_Transform;
		Rect      &m_Rect;
		std::uint8_t m_ColorR{0xFF}, m_ColorG{0xFF}, m_ColorB{0xFF};
	};
}// namespace roingine

#endif// RECT_RENDERER_H
