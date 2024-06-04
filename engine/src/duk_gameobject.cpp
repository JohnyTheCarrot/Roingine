#include "duk_gameobject.h"
#include "component_args.h"
#include <duktape.h>
#include <roingine/components/component.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace roingine::duk_gameobject {
	int GetComponent(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__pScene");
		auto *pScene{static_cast<Scene *>(duk_require_pointer(ctx, -1))};
		duk_pop(ctx);
		duk_get_prop_literal(ctx, -1, "__hGo");
		auto const hGo{duk_require_int(ctx, -1)};
		duk_pop_2(ctx);
		auto go{pScene->GetGameObjectPtr(hGo)};

		auto *comp{go->GetOptionalComponent(name)};
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
		duk_get_prop_literal(ctx, -1, "__pScene");
		auto *pScene{static_cast<Scene *>(duk_require_pointer(ctx, -1))};
		duk_pop(ctx);
		duk_get_prop_literal(ctx, -1, "__hGo");
		auto const hGo{duk_require_int(ctx, -1)};
		duk_pop_2(ctx);
		auto args{CollectDataFromDukArgs(ctx)};
		auto go{pScene->GetGameObjectPtr(hGo)};

		auto *comp{go->AddComponent(name, std::move(args))};

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
		duk_get_prop_literal(ctx, -1, "__pScene");
		auto *pScene{static_cast<Scene *>(duk_require_pointer(ctx, -1))};
		duk_pop(ctx);
		duk_get_prop_literal(ctx, -1, "__hGo");
		auto const hGo{duk_require_int(ctx, -1)};
		duk_pop_2(ctx);
		auto go{pScene->GetGameObjectPtr(hGo)};

		go->Destroy();

		return 0;
	}

	duk_function_list_entry const gameObjectFunctions[]{
	        {"getComponent", GetComponent, DUK_VARARGS},
	        {"addComponent", AddComponent, DUK_VARARGS},
	        {"destroy", Destroy, 0},
	        {nullptr, nullptr, 0}
	};

	void PushGameObject(GameObject const &pGameObject, duk_context *ctx) {
		duk_push_object(ctx);
		duk_push_pointer(ctx, pGameObject.GetScene());
		duk_put_prop_literal(ctx, -2, "__pScene");
		duk_push_int(ctx, static_cast<int>(pGameObject.GetHandle()));
		duk_put_prop_literal(ctx, -2, "__hGo");
		duk_put_function_list(ctx, -1, gameObjectFunctions);
	}

	void PutGameObject(DukObject &obj, GameObject const &pGameObject, std::string const &key, DukContext &ctx) {
		PushGameObject(pGameObject, ctx.GetRawContext());
		duk_put_prop_string(ctx.GetRawContext(), obj.GetStackIndex(), key.c_str());
	}
}// namespace roingine::duk_gameobject
