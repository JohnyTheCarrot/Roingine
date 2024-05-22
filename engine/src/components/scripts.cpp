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

		ptr->AddScript(fileName);

		return 0;
	}

	duk_function_list_entry const API[]{{"addScript", AddScript, 1}, {nullptr, nullptr, 0}};

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
}// namespace roingine
