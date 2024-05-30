#include "duk_gameobject.h"
#include <duktape.h>
#include <roingine/gameobject.h>

namespace roingine::duk_gameobject {
	int GetComponent(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		auto *ptr{static_cast<GameObject *>(duk_require_pointer(ctx, -1))};
		duk_pop(ctx);

		auto *comp{ptr->GetOptionalComponent(name)};
		if (!comp) {
			duk_push_undefined(ctx);
			return 1;
		}

		auto const api{comp->SetUpScriptAPI(ctx)};
		duk_push_object(ctx);
		duk_put_function_list(ctx, -1, api);
		duk_push_pointer(ctx, static_cast<void *>(comp));
		duk_put_prop_string(ctx, -2, "__ptr");

		return 1;
	}

	int AddComponent(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		auto *ptr{static_cast<GameObject *>(duk_require_pointer(ctx, -1))};
		duk_pop(ctx);

		auto *comp{ptr->AddComponent(name, ctx)};

		auto const api{comp->SetUpScriptAPI(ctx)};
		duk_push_object(ctx);

		if (api) {
			duk_put_function_list(ctx, -1, api);
		}

		duk_push_pointer(ctx, static_cast<void *>(comp));
		duk_put_prop_string(ctx, -2, "__ptr");

		return 1;
	}

	int Destroy(duk_context *ctx) {
		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		auto *ptr{static_cast<GameObject *>(duk_require_pointer(ctx, -1))};
		duk_pop(ctx);

		ptr->Destroy();

		return 0;
	}

	duk_function_list_entry const gameObjectFunctions[]{
	        {"getComponent", GetComponent, DUK_VARARGS},
	        {"addComponent", AddComponent, DUK_VARARGS},
	        {"destroy", Destroy, 0},
	        {nullptr, nullptr, 0}
	};

	void PushGameObject(GameObject *pGameObject, DukContext &ctx) {
		duk_push_object(ctx.GetRawContext());
		duk_push_pointer(ctx.GetRawContext(), pGameObject);
		duk_put_prop_literal(ctx.GetRawContext(), -2, "__ptr");
		duk_put_function_list(ctx.GetRawContext(), -1, gameObjectFunctions);
	}

	void PutGameObject(DukObject &obj, GameObject *pGameObject, std::string const &key, DukContext &ctx) {
		PushGameObject(pGameObject, ctx);
		duk_put_prop_string(ctx.GetRawContext(), obj.GetStackIndex(), key.c_str());
	}


}// namespace roingine::duk_gameobject
