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
		auto const fileName{duk_require_string(ctx, -1)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Scripts *ptr{static_cast<Scripts *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		ptr->AddScript(fileName);

		return 0;
	}

	int GetScript(duk_context *ctx) {
		auto const scriptName{duk_require_string(ctx, -1)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Scripts *ptr{static_cast<Scripts *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		auto script{ptr->GetScript(scriptName)};
		if (script == nullptr) {
			duk_push_undefined(ctx);
			return 1;
		}

		script->ReturnAPIObject(ctx);

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

	std::unique_ptr<Scripts> Scripts::JSFactory(GameObject *pGameObject, duk_context *) {
		return std::make_unique<Scripts>(*pGameObject);
	}

	void Scripts::AddScript(std::string_view fileName) {
		try {
			Script script{GetGameObject(), fileName};

			m_Scripts.emplace(script.GetScriptName(), std::move(script));
		} catch (ScriptCompilationFailedException const &ex) {
			std::cerr << "Failed to compile script \"" << fileName << "\": " << ex.what() << std::endl;
		} catch (FatalScriptError const &ex) {
			std::cerr << "Fatal script error in \"" << fileName << "\": " << ex.what() << std::endl;
		}
	}

	Script *Scripts::GetScript(std::string const &name) {
		if (!m_Scripts.contains(name))
			return nullptr;

		return &m_Scripts.at(name);
	}

	void Scripts::ExecuteOnEveryScript(std::function<void(GameObject gameObject, DukContext &)> const &fn) {
		std::for_each(m_Scripts.begin(), m_Scripts.end(), [&](auto &pair) {
			fn(pair.second.GetGameObject(), pair.second.GetDukContext());
		});
	}
}// namespace roingine
