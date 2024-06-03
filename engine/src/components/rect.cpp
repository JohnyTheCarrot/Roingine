#include <duktape.h>
#include <roingine/components/rect.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <stdexcept>

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

	char const *Rect::GetName() const {
		return NAME;
	}

	duk_function_list_entry const *Rect::SetUpScriptAPI(duk_context *) const {
		return nullptr;
	}

	std::unique_ptr<Rect> Rect::JSFactory(GameObject *pGameObject, std::vector<ComponentInitArgument> const &args) {
		auto const width{comp_init::RequireDouble(0, args)};
		auto const height{comp_init::RequireDouble(0, args)};

		return std::make_unique<Rect>(*pGameObject, static_cast<float>(width), static_cast<float>(height));
	}

	float Rect::GetWidth() const noexcept {
		return m_Width;
	}

	float Rect::GetHeight() const noexcept {
		return m_Height;
	}

	void Rect::SetWidth(float width) {
		if (width <= 0.f) {
			throw std::invalid_argument{"Width must be > 0"};
		}

		m_Width = width;
	}

	void Rect::SetHeight(float height) {
		if (height <= 0.f) {
			throw std::invalid_argument{"Height must be > 0"};
		}

		m_Height = height;
	}
}// namespace roingine
