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

			CallJSCallback(hitPoint);
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

		return 0;
	}

	duk_function_list_entry const API[]{{"onCollision", OnCollision, 1}, {nullptr, nullptr, 0}};

	duk_function_list_entry const *RectCollider::SetUpScriptAPI(duk_context *) const {
		return API;
	}

	std::unique_ptr<RectCollider> RectCollider::JSFactory(GameObject *pGameObject, duk_context *ctx) {
		auto const width{duk_require_number(ctx, 1)};
		auto const height{duk_require_number(ctx, 2)};

		return std::make_unique<RectCollider>(*pGameObject, static_cast<float>(width), static_cast<float>(height));
	}

	void RectCollider::CallJSCallback(glm::vec2 hitPoint) {
		auto *pScripts{GetGameObject().GetOptionalComponent<Scripts>()};
		if (pScripts == nullptr)
			return;

		pScripts->ExecuteOnEveryScript([&](duk_context *ctx) {
			duk_push_global_stash(ctx);

			duk_get_prop_string(ctx, -1, RectCollider::JS_CALLBACK_NAME);

			if (duk_is_undefined(ctx, -1)) {
				duk_pop_2(ctx);
				return;
			}

			duk_push_number(ctx, hitPoint.x);
			duk_push_number(ctx, hitPoint.y);
			if (duk_pcall(ctx, 2) != 0) {
				std::cerr << "Error calling rect collider callback: " << duk_safe_to_string(ctx, -1) << std::endl;
			}
			duk_pop_2(ctx);
		});
	}
}// namespace roingine
