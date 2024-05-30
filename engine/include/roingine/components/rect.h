#ifndef RECT_H
#define RECT_H

#include <array>
#include <glm/glm.hpp>
#include <roingine/components/component.h>

namespace roingine {
	class Transform;

	struct Rect final : public Component {
		using Corners = std::array<glm::vec2, 4>;

		Rect(GameObject &gameObject, float width, float height);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		static constexpr char const *NAME{"Rect"};

		[[nodiscard]]
		char const *GetName() const override;

		[[nodiscard]]
		duk_function_list_entry const *SetUpScriptAPI(duk_context *) const override;

		[[nodiscard]]
		static std::unique_ptr<Rect> JSFactory(GameObject *, duk_context *);

		[[nodiscard]]
		float GetWidth() const noexcept;

		[[nodiscard]]
		float GetHeight() const noexcept;

		void SetWidth(float width);

		void SetHeight(float height);

	private:
		float m_Width, m_Height;
	};
}// namespace roingine

#endif// RECT_H
