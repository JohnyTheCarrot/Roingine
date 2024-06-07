#include "duk_gameobject.h"
#include "component_args.h"
#include <duktape.h>
#include <iostream>
#include <roingine/components/component.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>
#include <roingine/script.h>

namespace roingine::duk_gameobject {
	GameObject *GetGameObjectPtr(duk_context *ctx) {
		duk_push_global_object(ctx);
		duk_get_prop_literal(ctx, -1, "__scriptPtr");
		auto *pScript{static_cast<Script *>(duk_require_pointer(ctx, -1))};
		duk_pop_2(ctx);

		auto pScene{pScript->GetGameObject().GetScene()};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "handle");
		auto const hGo{duk_require_int(ctx, -1)};
		duk_pop_2(ctx);
		return pScene->GetGameObjectPtr(hGo);
	}

	int GetComponent(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};
		auto       go{GetGameObjectPtr(ctx)};

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
		auto       go{GetGameObjectPtr(ctx)};

		auto args{CollectDataFromDukArgs(ctx)};

		auto *comp{go->AddComponent(name, std::move(args))};
		if (comp == nullptr) {
			std::cerr << "AddComponent of " << name << " failed" << std::endl;
			return 0;
		}

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
		auto go{GetGameObjectPtr(ctx)};

		go->Destroy();

		return 0;
	}

	int SetLabel(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};
		auto       go{GetGameObjectPtr(ctx)};

		go->SetLabel(name);

		return 0;
	}

	int GetLabel(duk_context *ctx) {
		auto go{GetGameObjectPtr(ctx)};

		auto const label{go->GetLabel()};
		if (label.has_value())
			duk_push_string(ctx, label->data());
		else
			duk_push_null(ctx);

		return 1;
	}

	int HasLabel(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};
		auto       go{GetGameObjectPtr(ctx)};

		auto const label{go->GetLabel()};
		duk_push_boolean(ctx, label == name);

		return 1;
	}

	int SetID(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};
		auto       go{GetGameObjectPtr(ctx)};

		go->SetUniqueID(name);

		return 0;
	}

	int GetID(duk_context *ctx) {
		auto go{GetGameObjectPtr(ctx)};

		auto const uniqueId{go->GetUniqueID()};
		if (uniqueId.has_value())
			duk_push_string(ctx, uniqueId->data());
		else
			duk_push_null(ctx);

		return 1;
	}

	int HasID(duk_context *ctx) {
		auto const name{duk_require_string(ctx, 0)};
		auto       go{GetGameObjectPtr(ctx)};

		auto const uniqueId{go->GetUniqueID()};
		duk_push_boolean(ctx, uniqueId == name);

		return 1;
	}

	int SetEnabled(duk_context *ctx) {
		auto const enabled{duk_require_boolean(ctx, 0)};
		auto       go{GetGameObjectPtr(ctx)};

		go->SetEnabled(enabled);

		return 0;
	}

	int GetEnabled(duk_context *ctx) {
		auto       go{GetGameObjectPtr(ctx)};
		auto const enabled{go->GetEnabled()};

		duk_push_boolean(ctx, enabled);

		return 1;
	}

	duk_function_list_entry const gameObjectFunctions[]{
	        {"getComponent", GetComponent, DUK_VARARGS},
	        {"addComponent", AddComponent, DUK_VARARGS},
	        {"destroy", Destroy, 0},
	        {"setLabel", SetLabel, 1},
	        {"getLabel", GetLabel, 0},
	        {"hasLabel", HasLabel, 1},
	        {"setId", SetID, 1},
	        {"getId", GetID, 0},
	        {"hasId", HasID, 1},
	        {"setEnabled", SetEnabled, 1},
	        {"getEnabled", GetEnabled, 0},
	        {nullptr, nullptr, 0}
	};

	void PushGameObject(GameObject const &pGameObject, duk_context *ctx) {
		duk_push_object(ctx);
		duk_push_int(ctx, static_cast<int>(pGameObject.GetHandle()));
		duk_put_prop_literal(ctx, -2, "handle");
		duk_put_function_list(ctx, -1, gameObjectFunctions);
	}

	void PutGameObject(DukObject &obj, GameObject const &pGameObject, std::string const &key, DukContext &ctx) {
		PushGameObject(pGameObject, ctx.GetRawContext());
		duk_put_prop_string(ctx.GetRawContext(), obj.GetStackIndex(), key.c_str());
	}
}// namespace roingine::duk_gameobject
