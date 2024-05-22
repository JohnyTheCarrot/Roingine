#include <duktape.h>
#include <format>
#include <fstream>
#include <iostream>
#include <roingine/commands/command.h>
#include <roingine/game_time.h>
#include <roingine/gameobject.h>
#include <roingine/input.h>
#include <roingine/roingine.h>
#include <roingine/script.h>

namespace roingine {
	std::string KeyEventListenerKey(KeyEventType eventType, InputKeys key) {
		return std::format("_{}_{}", static_cast<int>(eventType), static_cast<int>(key));
	}

	class ScriptPressCommand final : public Command {
	public:
		ScriptPressCommand(duk_context *dukContext, KeyEventType eventType, InputKeys key)
		    : m_Key{KeyEventListenerKey(eventType, key)}
		    , m_DukContext{dukContext} {
		}

		void Execute() override {
			duk_push_global_stash(m_DukContext);

			duk_get_prop_string(m_DukContext, -1, m_Key.c_str());
			if (duk_pcall(m_DukContext, 0) != 0)
				std::cerr << "Error: " << duk_safe_to_string(m_DukContext, -1) << std::endl;
			duk_pop(m_DukContext);
			duk_pop(m_DukContext);
		}

	private:
		std::string  m_Key;
		duk_context *m_DukContext;
	};

	int Print(duk_context *dukContext) {
		auto numArgs{duk_get_top(dukContext)};

		if (numArgs--) {
			std::cout << duk_to_string(dukContext, -numArgs - 1);
		}
		while (numArgs--) { std::cout << ' ' << duk_to_string(dukContext, -numArgs - 1); }

		return 0;
	}

	int PrintLn(duk_context *dukContext) {
		auto const result{Print(dukContext)};

		std::cout << std::endl;

		return result;
	}

	int Listen(duk_context *dukContext, KeyEventType eventType) {
		auto const key{duk_to_int32(dukContext, 0)};
		if (key < 0 || key > static_cast<int>(InputKeys::NumKeys)) {
			return 0;
		}
		auto const inputKey{static_cast<InputKeys>(key)};
		duk_push_global_stash(dukContext);
		duk_dup(dukContext, 1);
		std::string name{KeyEventListenerKey(eventType, inputKey)};

		duk_put_prop_string(dukContext, -2, name.c_str());
		duk_pop(dukContext);

		KeyboardInput::GetService().AddCommand(
		        inputKey, eventType, std::make_unique<ScriptPressCommand>(dukContext, eventType, inputKey)
		);

		return 0;
	}

	int GetDeltaTime(duk_context *dukContext) {
		auto const deltaTime{GameTime::GetInstance().GetDeltaTime()};
		duk_push_number(dukContext, deltaTime);

		return 1;
	}

	int GetComponent(duk_context *dukContext) {
		auto const name{duk_require_string(dukContext, 0)};

		duk_get_global_literal(dukContext, "__goPtr");
		auto *ptr{static_cast<GameObject *>(duk_get_pointer(dukContext, -1))};

		auto *comp{ptr->GetComponent(name)};
		if (!comp) {
			duk_push_undefined(dukContext);
			return 1;
		}

		auto const api{comp->SetUpScriptAPI(dukContext)};
		duk_push_object(dukContext);
		duk_put_function_list(dukContext, -1, api);
		duk_push_pointer(dukContext, static_cast<void *>(comp));
		duk_put_prop_string(dukContext, -2, "__ptr");

		return 1;
	}

	int AddComponent(duk_context *dukContext) {
		auto const name{duk_require_string(dukContext, 0)};

		duk_get_global_literal(dukContext, "__goPtr");
		auto *ptr{static_cast<GameObject *>(duk_get_pointer(dukContext, -1))};

		auto *comp{ptr->AddComponent(name, dukContext)};

		auto const api{comp->SetUpScriptAPI(dukContext)};
		duk_push_object(dukContext);

		if (api) {
			duk_put_function_list(dukContext, -1, api);
		}

		duk_push_pointer(dukContext, static_cast<void *>(comp));
		duk_put_prop_string(dukContext, -2, "__ptr");

		return 1;
	}

	int OnKeyDown(duk_context *dukContext) {
		return Listen(dukContext, KeyEventType::Down);
	}

	int OnKeyUp(duk_context *dukContext) {
		return Listen(dukContext, KeyEventType::Up);
	}

	int OnKeyHeld(duk_context *dukContext) {
		return Listen(dukContext, KeyEventType::Held);
	}

	int OnKeyLongPress(duk_context *dukContext) {
		return Listen(dukContext, KeyEventType::LongPress);
	}

	void Script::CallJsFunctionByName(std::string_view name) {
		duk_get_global_string(m_DukContext.get(), name.data());
		if (duk_is_undefined(m_DukContext.get(), -1)) {
			duk_pop(m_DukContext.get());
			return;
		}

		duk_push_number(m_DukContext.get(), static_cast<duk_double_t>(GameTime::GetInstance().GetDeltaTime()));
		if (duk_pcall(m_DukContext.get(), 1) != 0)
			std::cerr << "Error: " << duk_safe_to_string(m_DukContext.get(), -1) << std::endl;
		duk_pop(m_DukContext.get());
	}

	duk_function_list_entry const roingineFunctions[]{
	        {"println", PrintLn, DUK_VARARGS},
	        {"print", Print, DUK_VARARGS},
	        {"getDeltaTime", GetDeltaTime, 0},
	        {nullptr, nullptr, 0}
	};

	duk_function_list_entry const gameObjectFunctions[]{
	        {"getComponent", GetComponent, DUK_VARARGS},
	        {"addComponent", AddComponent, DUK_VARARGS},
	        {nullptr, nullptr, 0}
	};

	duk_function_list_entry const inputFunctions[]{
	        {"onKeyDown", OnKeyDown, 2},
	        {"onKeyUp", OnKeyUp, 2},
	        {"onKeyHeld", OnKeyHeld, 2},
	        {"onKeyLongPressed", OnKeyLongPress, 2},
	        {nullptr, nullptr, 0}
	};

	duk_number_list_entry const inputNumberConstants[]{
	        {"KEY_A", static_cast<int>(InputKeys::A)},         {"KEY_B", static_cast<int>(InputKeys::B)},
	        {"KEY_C", static_cast<int>(InputKeys::C)},         {"KEY_D", static_cast<int>(InputKeys::D)},
	        {"KEY_E", static_cast<int>(InputKeys::E)},         {"KEY_F", static_cast<int>(InputKeys::F)},
	        {"KEY_G", static_cast<int>(InputKeys::G)},         {"KEY_H", static_cast<int>(InputKeys::H)},
	        {"KEY_I", static_cast<int>(InputKeys::I)},         {"KEY_J", static_cast<int>(InputKeys::J)},
	        {"KEY_K", static_cast<int>(InputKeys::K)},         {"KEY_L", static_cast<int>(InputKeys::L)},
	        {"KEY_M", static_cast<int>(InputKeys::M)},         {"KEY_N", static_cast<int>(InputKeys::N)},
	        {"KEY_O", static_cast<int>(InputKeys::O)},         {"KEY_P", static_cast<int>(InputKeys::P)},
	        {"KEY_Q", static_cast<int>(InputKeys::Q)},         {"KEY_R", static_cast<int>(InputKeys::R)},
	        {"KEY_S", static_cast<int>(InputKeys::S)},         {"KEY_T", static_cast<int>(InputKeys::T)},
	        {"KEY_U", static_cast<int>(InputKeys::U)},         {"KEY_V", static_cast<int>(InputKeys::V)},
	        {"KEY_W", static_cast<int>(InputKeys::W)},         {"KEY_X", static_cast<int>(InputKeys::X)},
	        {"KEY_Y", static_cast<int>(InputKeys::Y)},         {"KEY_Z", static_cast<int>(InputKeys::Z)},
	        {"KEY_SPACE", static_cast<int>(InputKeys::Space)}, {"KEY_ENTER", static_cast<int>(InputKeys::Enter)},
	        {"KEY_SHIFT", static_cast<int>(InputKeys::Shift)}, {nullptr, 0}
	};

	static void my_fatal(void *, char const *msg) {
		throw FatalScriptError{msg};
	}

	Script::Script(GameObject gameObject, std::string_view fileName)
	    : m_GameObject{gameObject}
	    , m_DukContext{duk_create_heap(nullptr, nullptr, nullptr, nullptr, my_fatal)} {
		std::ifstream ifstream{fileName.data()};
		if (!ifstream)
			throw ScriptCompilationFailedException{"File not found"};

		duk_push_global_object(m_DukContext.get());
		duk_push_object(m_DukContext.get());
		duk_put_function_list(m_DukContext.get(), -1, roingineFunctions);
		duk_put_prop_string(m_DukContext.get(), -2, "roingine");

		duk_push_object(m_DukContext.get());
		duk_put_function_list(m_DukContext.get(), -1, gameObjectFunctions);
		duk_put_prop_string(m_DukContext.get(), -2, "gameObject");

		duk_push_object(m_DukContext.get());
		duk_put_number_list(m_DukContext.get(), -1, inputNumberConstants);
		duk_put_function_list(m_DukContext.get(), -1, inputFunctions);
		duk_put_prop_string(m_DukContext.get(), -2, "input");

		duk_push_pointer(m_DukContext.get(), static_cast<void *>(&gameObject));
		duk_put_prop_string(m_DukContext.get(), -2, "__goPtr");
		duk_pop(m_DukContext.get());

		std::string inputCode{std::istreambuf_iterator<char>(ifstream), {}};

		if (duk_peval_string(m_DukContext.get(), inputCode.c_str()) != 0) {
			auto const errMessage{duk_safe_to_string(m_DukContext.get(), -1)};
			throw ScriptCompilationFailedException{errMessage};
		}

		CallJsFunctionByName("Init");
		duk_get_global_literal(m_DukContext.get(), "SCRIPT_NAME");
		auto const scriptName{duk_get_string(m_DukContext.get(), -1)};

		if (!scriptName) {
			throw FatalScriptError{"A script is required to declare a SCRIPT_NAME string."};
		}

		m_ScriptName = scriptName;
	}

	Script::Script(Script &&other)
	    : m_GameObject{other.m_GameObject}
	    , m_ListenedToKeys{std::move(other.m_ListenedToKeys)}
	    , m_DukContext{std::move(other.m_DukContext)}
	    , m_ScriptName{std::move(other.m_ScriptName)} {
		duk_push_global_object(m_DukContext.get());
		duk_push_pointer(m_DukContext.get(), static_cast<void *>(&m_GameObject));
		duk_put_prop_string(m_DukContext.get(), -2, "__goPtr");
		duk_pop(m_DukContext.get());
	}

	Script &Script::operator=(Script &&other) {
		if (this == &other)
			return *this;

		m_ListenedToKeys = std::move(other.m_ListenedToKeys);
		m_DukContext     = std::move(other.m_DukContext);
		m_ScriptName     = std::move(other.m_ScriptName);

		duk_push_global_object(m_DukContext.get());
		duk_push_pointer(m_DukContext.get(), static_cast<void *>(&m_GameObject));
		duk_put_prop_string(m_DukContext.get(), -2, "__goPtr");
		duk_pop(m_DukContext.get());

		return *this;
	}

	void Script::Update() {
		CallJsFunctionByName("Update");
	}

	void Script::FixedUpdate() {
		CallJsFunctionByName("FixedUpdate");
	}

	void Script::Render() const {
	}

	ScriptCompilationFailedException::ScriptCompilationFailedException(std::string errorMessage)
	    : m_ErrorMessage{std::move(errorMessage)} {
	}

	char const *ScriptCompilationFailedException::what() const noexcept {
		return m_ErrorMessage.c_str();
	}

	std::string_view Script::GetScriptName() const {
		return m_ScriptName;
	}

	FatalScriptError::FatalScriptError(std::string errorMessage)
	    : m_ErrorMessage{std::move(errorMessage)} {
	}

	char const *FatalScriptError::what() const noexcept {
		return m_ErrorMessage.c_str();
	}
}// namespace roingine
