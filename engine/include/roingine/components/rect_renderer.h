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
		static std::size_t JSFactoryNumParams();

		[[nodiscard]]
		static std::unique_ptr<RectRenderer> JSFactory(GameObject *, duk_context *);

	private:
		Transform &m_Transform;
		Rect      &m_Rect;
	};
}// namespace roingine

#endif// RECT_RENDERER_H
