#ifndef RECT_COLLIDER_H
#define RECT_COLLIDER_H

#include <memory>
#include <roingine/components/component.h>

namespace roingine {
	class Transform;

	class RectCollider final : public Component {
	public:
		RectCollider(GameObject &gameObject, float width, float height);

		void Update() override;

		void PostUpdate() override;

		void FixedUpdate() override;

		void Render() const override;

		static constexpr char const *NAME{"RectCollider"};

		static constexpr char const *JS_CALLBACK_NAME{"rectColliderCallback"};

		char const *GetName() const override;

		duk_function_list_entry const *SetUpScriptAPI(duk_context *ctx) const override;

		[[nodiscard]]
		static std::unique_ptr<RectCollider> JSFactory(GameObject *pGameObject, duk_context *ctx);

	private:
		void CallJSCallback(glm::vec2 hitPoint);

		Transform &m_Transform;
		float      m_Width, m_Height;
	};
}// namespace roingine

#endif// RECT_COLLIDER_H
