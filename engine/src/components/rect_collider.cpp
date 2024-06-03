#include "duk_gameobject.h"
#include <duktape.h>
#include <iostream>
#include <roingine/components/rect.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/scripts.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace roingine {
	RectCollider::RectCollider(GameObject &gameObject, float width, float height)
	    : Component(gameObject)
	    , m_Transform{gameObject.GetComponent<Transform>()}
	    , m_Width{width}
	    , m_Height{height} {
	}

	void RectCollider::Update() {
	}

	void RectCollider::PostUpdate() {
		if (!m_HasListener)
			return;

		auto const worldPos{m_Transform.GetWorldPosition()};

		GetGameObject().GetScene()->ForEveryComponentOfType<RectCollider>([&](RectCollider &other) {
			if (&other == this)
				return;

			auto const otherWorldPos{other.m_Transform.GetWorldPosition()};

			auto const left{worldPos.x};
			auto const right{worldPos.x + m_Width};
			auto const top{worldPos.y + m_Height};
			auto const bottom{worldPos.y};

			auto const otherLeft{otherWorldPos.x};
			auto const otherRight{otherWorldPos.x + other.m_Width};
			auto const otherTop{otherWorldPos.y + other.m_Height};
			auto const otherBottom{otherWorldPos.y};

			if (right <= otherLeft || left >= otherRight)
				return;

			if (bottom >= otherTop || top <= otherBottom)
				return;

			auto const intersectTop{top > otherTop && bottom < otherTop};
			auto const intersectBottom{bottom < otherBottom && top > otherBottom};
			auto const intersectLeft{left < otherLeft && right > otherLeft};
			auto const intersectRight{right > otherRight && left < otherRight};

			auto const distanceTop{std::abs(top - otherTop)};
			auto const distanceBottom{std::abs(bottom - otherBottom)};
			auto const smallestXDistance{std::min(std::abs(left - otherLeft), std::abs(right - otherRight))};

			glm::vec2 hitPoint{worldPos};
			if (intersectTop && distanceTop > smallestXDistance)
				hitPoint.y = otherTop;
			else if (intersectBottom && distanceBottom > smallestXDistance)
				hitPoint.y = otherBottom - m_Height;
			else if (intersectLeft)
				hitPoint.x = otherLeft - m_Width;
			else if (intersectRight)
				hitPoint.x = otherRight;

			CallJSCallback(other.GetGameObject(), hitPoint);
		});
	}

	void RectCollider::FixedUpdate() {
	}

	void RectCollider::Render() const {
	}

	char const *RectCollider::GetName() const {
		return NAME;
	}

	int OnCollision(duk_context *ctx) {
		duk_push_global_stash(ctx);

		duk_dup(ctx, -2);
		duk_require_function(ctx, -1);
		duk_put_prop_string(ctx, -2, RectCollider::JS_CALLBACK_NAME);
		duk_pop(ctx);

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		RectCollider *ptr{static_cast<RectCollider *>(duk_get_pointer(ctx, -1))};
		ptr->SetHasListener(true);

		return 0;
	}

	duk_function_list_entry const API[]{{"onCollision", OnCollision, 1}, {nullptr, nullptr, 0}};

	duk_function_list_entry const *RectCollider::SetUpScriptAPI(duk_context *) const {
		return API;
	}

	std::unique_ptr<RectCollider>
	RectCollider::JSFactory(GameObject *pGameObject, std::vector<ComponentInitArgument> const &args) {
		auto const width{comp_init::RequireDouble(0, args)};
		auto const height{comp_init::RequireDouble(1, args)};

		return std::make_unique<RectCollider>(*pGameObject, static_cast<float>(width), static_cast<float>(height));
	}

	bool RectCollider::GetHasListener() const noexcept {
		return m_HasListener;
	}

	void RectCollider::SetHasListener(bool hasListener) noexcept {
		m_HasListener = hasListener;
	}

	void RectCollider::CallJSCallback(GameObject otherGameObject, glm::vec2 hitPoint) {
		auto *pScripts{GetGameObject().GetOptionalComponent<Scripts>()};
		if (pScripts == nullptr)
			return;

		pScripts->ExecuteOnEveryScript([&](DukContext &ctx) {
			duk_push_global_stash(ctx.GetRawContext());

			duk_get_prop_string(ctx.GetRawContext(), -1, RectCollider::JS_CALLBACK_NAME);

			if (duk_is_undefined(ctx.GetRawContext(), -1)) {
				duk_pop_2(ctx.GetRawContext());
				return;
			}

			duk_gameobject::PushGameObject(otherGameObject, ctx.GetRawContext());
			duk_push_number(ctx.GetRawContext(), hitPoint.x);
			duk_push_number(ctx.GetRawContext(), hitPoint.y);
			if (duk_pcall(ctx.GetRawContext(), 3) != 0) {
				std::cerr << "Error calling rect collider callback: " << duk_safe_to_string(ctx.GetRawContext(), -1)
				          << std::endl;
			}
			duk_pop_2(ctx.GetRawContext());
		});
	}
}// namespace roingine
