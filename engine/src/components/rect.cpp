#include <duktape.h>
#include <roingine/components/rect.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>

namespace roingine {
	Rect::Rect(GameObject &gameObject, float width, float height)
	    : Component{gameObject}
	    , m_Width{width}
	    , m_Height{height} {
	}

	void Rect::Update() {
	}

	void Rect::FixedUpdate() {
	}

	void Rect::Render() const {
	}

	Corners Rect::GetCorners(Transform &transform) const noexcept {
		auto const worldPos{transform.GetWorldPosition()};
		auto const minX{worldPos.x};
		auto const maxX{worldPos.x + m_Width};
		auto const minY{worldPos.y};
		auto const maxY{worldPos.y + m_Height};

		return {.topLeft = worldPos, .bottomLeft = {minX, maxY}, .topRight = {maxX, minY}, .bottomRight = {maxX, maxY}};
	}

	char const *Rect::GetName() const {
		return NAME;
	}

	duk_function_list_entry const *Rect::SetUpScriptAPI(duk_context *) const {
		return nullptr;
	}

	size_t Rect::JSFactoryNumParams() {
		return 2;
	}

	std::unique_ptr<Rect> Rect::JSFactory(GameObject *pGameObject, duk_context *ctx) {
		auto const width{duk_require_number(ctx, 1)};
		auto const height{duk_require_number(ctx, 2)};

		return std::make_unique<Rect>(*pGameObject, static_cast<float>(width), static_cast<float>(height));
	}
}// namespace roingine
