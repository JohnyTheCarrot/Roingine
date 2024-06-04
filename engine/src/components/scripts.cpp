#include "component_args.h"
#include <algorithm>
#include <duktape.h>
#include <iostream>
#include <roingine/components/scripts.h>

namespace roingine {
	Scripts::Scripts(GameObject &gameObject)
	    : Component{gameObject} {
	}

	void Scripts::Update() {
		for (auto &script: m_Scripts) { script.second.Update(); }
	}

	void Scripts::FixedUpdate() {
		for (auto &script: m_Scripts) { script.second.FixedUpdate(); }
	}

	void Scripts::Render() const {
		for (auto const &script: m_Scripts) { script.second.Render(); }
	}

	char const *Scripts::GetName() const {
		return NAME;
	}

	int AddScript(duk_context *ctx) {
		auto const fileName{duk_require_string(ctx, 0)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Scripts *ptr{static_cast<Scripts *>(duk_get_pointer(ctx, -1))};
		duk_pop_2(ctx);

		auto args{CollectDataFromDukArgs(ctx)};

		auto pScript{ptr->AddScript(fileName, args)};
		pScript->ReturnAPIObject(ctx);

		return 1;
	}

	int GetScript(duk_context *ctx) {
		auto const scriptName{duk_require_string(ctx, -1)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Scripts *ptr{static_cast<Scripts *>(duk_get_pointer(ctx, -1))};
		duk_pop_2(ctx);

		auto pScript{ptr->GetScript(scriptName)};
		if (pScript == nullptr) {
			duk_push_undefined(ctx);
			return 1;
		}

		pScript->ReturnAPIObject(ctx);

		return 1;
	}

	duk_function_list_entry const API[]{
	        {"addScript", AddScript, 1},
	        {"getScript", GetScript, 1},
	        {nullptr, nullptr, 0}
	};

	duk_function_list_entry const *Scripts::SetUpScriptAPI(duk_context *) const {
		return API;
	}

	std::unique_ptr<Scripts> Scripts::JSFactory(GameObject *pGameObject, std::vector<JSData> const &) {
		return std::make_unique<Scripts>(*pGameObject);
	}

	Script *Scripts::AddScript(
	        std::string_view fileName, std::vector<JSData> const &args,
	        std::optional<Script::CppFunctionCaller> const &caller
	) {
		try {
			Script script{*this, fileName, args, caller};

			auto pair{m_Scripts.emplace(script.GetScriptName(), std::move(script)).first};

			return &pair->second;
		} catch (ScriptCompilationFailedException const &ex) {
			std::cerr << "Failed to compile script \"" << fileName << "\": " << ex.what() << std::endl;
		} catch (FatalScriptError const &ex) {
			std::cerr << "Fatal script error in \"" << fileName << "\": " << ex.what() << std::endl;
		}

		return nullptr;
	}

	Script *Scripts::GetScript(std::string const &name) {
		if (!m_Scripts.contains(name))
			return nullptr;

		return &m_Scripts.at(name);
	}

	void Scripts::ExecuteOnEveryScript(std::function<void(DukContext &)> const &fn) {
		std::for_each(m_Scripts.begin(), m_Scripts.end(), [&](auto &pair) { fn(pair.second.GetDukContext()); });
	}
}// namespace roingine
