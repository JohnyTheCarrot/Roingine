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
			duk_pop_2(m_DukContext);
		}

	private:
		std::string  m_Key;
		duk_context *m_DukContext;
	};

	int Print(duk_context *dukContext) {
		auto numArgs{duk_get_top(dukContext)};

		auto Print{[dukContext, &numArgs]() {
			auto const index{-numArgs - 1};
			auto const isObject{duk_check_type(dukContext, index, DUK_TYPE_OBJECT)};

			if (isObject) {
				std::cout << duk_json_encode(dukContext, index);
			} else {
				std::cout << duk_to_string(dukContext, index);
			}
		}};

		if (numArgs--) {
			Print();
		}
		while (numArgs--) {
			std::cout << ' ';
			Print();
		}

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

		auto pCommand{std::make_unique<ScriptPressCommand>(dukContext, eventType, inputKey)};

		duk_get_global_literal(dukContext, "__scriptPtr");
		Script *ptr{static_cast<Script *>(duk_get_pointer(dukContext, -1))};
		ptr->RegisterListenedToKey(inputKey, eventType, std::move(pCommand));
		duk_pop(dukContext);

		return 0;
	}

	int GetDeltaTime(duk_context *dukContext) {
		auto const deltaTime{GameTime::GetInstance().GetDeltaTime()};
		duk_push_number(dukContext, deltaTime);

		return 1;
	}

	int ReadFile(duk_context *ctx) {
		auto                       fileName{std::string{duk_require_string(ctx, 0)}};
		constexpr std::string_view ABSOLUTE_PREFIX{"@/"};

		bool const isAbsolute{fileName.starts_with(ABSOLUTE_PREFIX)};
		if (isAbsolute)
			fileName = fileName.substr(ABSOLUTE_PREFIX.length());

		duk_get_global_literal(ctx, "__scriptPtr");
		Script *ptr{static_cast<Script *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		auto const &parentPath{ptr->GetPath().parent_path()};

		std::filesystem::path filePath{isAbsolute ? fileName : parentPath / fileName};

		std::ifstream iFileStream{filePath};

		if (!iFileStream) {
			std::cerr << "Could not load file " << fileName << std::endl;
			return DUK_RET_ERROR;
		}

		std::string inputCode{std::istreambuf_iterator<char>(iFileStream), {}};

		duk_push_string(ctx, inputCode.c_str());
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

	duk_function_list_entry const roingineFunctions[]{
	        {"println", PrintLn, DUK_VARARGS},
	        {"print", Print, DUK_VARARGS},
	        {"getDeltaTime", GetDeltaTime, 0},
	        {"readFile", ReadFile, 1},
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

	Script::Script(GameObject gameObject, std::string_view fileName)
	    : m_FilePath{fileName}
	    , m_GameObject{gameObject}
	    , m_DukContext{} {
		std::ifstream ifstream{fileName.data()};
		if (!ifstream)
			throw ScriptCompilationFailedException{"File not found"};

		{
			auto globalObject{m_DukContext.PushGlobalObject()};

			{
				auto roingineObject{globalObject.PushObject("roingine")};
				roingineObject.PutFunctionList(roingineFunctions);
			}

			{
				auto gameObjectObject{globalObject.PushObject("gameObject")};
				gameObjectObject.PutFunctionList(gameObjectFunctions);
			}

			{
				auto inputObject{globalObject.PushObject("input")};
				inputObject.PutFunctionList(inputFunctions);
				inputObject.PutNumberList(inputNumberConstants);
			}

			{
				auto currentObject{globalObject.PushObject("current")};
				currentObject.PushObject("properties");
			}

			globalObject.PushPointer("__goPtr", static_cast<void *>(&gameObject));
			globalObject.PushPointer("__scriptPtr", static_cast<void *>(this));
		}

		std::string const modSearchCode{"Duktape.modSearch=function(fileName){return roingine.readFile(fileName);};"};
		std::string inputCode{std::istreambuf_iterator<char>(ifstream), {}};

		m_DukContext.Eval(modSearchCode + std::move(inputCode));

		auto const scriptName{m_DukContext.GetGlobalString("SCRIPT_NAME")};

		if (!scriptName) {
			throw FatalScriptError{"A script is required to declare a SCRIPT_NAME string."};
		}

		m_ScriptName = scriptName;

		duk_get_global_literal(m_DukContext.GetRawContext(), "Init");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop(m_DukContext.GetRawContext());
			return;
		}
		if (duk_pcall(m_DukContext.GetRawContext(), 0) != 0)
			std::cerr << "Error: " << duk_safe_to_string(m_DukContext.GetRawContext(), -1) << std::endl;
		duk_pop(m_DukContext.GetRawContext());
	}

	Script::Script(Script &&other)
	    : m_GameObject{other.m_GameObject}
	    , m_ListenedToKeys{std::move(other.m_ListenedToKeys)}
	    , m_DukContext{std::move(other.m_DukContext)}
	    , m_ScriptName{std::move(other.m_ScriptName)} {
		auto global{m_DukContext.PushGlobalObject()};
		global.PushPointer("__goPtr", static_cast<void *>(&m_GameObject));
	}

	Script::~Script() {
		if (m_DukContext.GetRawContext() == nullptr)
			return;

		duk_get_global_literal(m_DukContext.GetRawContext(), "OnDispose");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop(m_DukContext.GetRawContext());
			return;
		}
		if (duk_pcall(m_DukContext.GetRawContext(), 0) != 0)
			std::cerr << "Error: " << duk_safe_to_string(m_DukContext.GetRawContext(), -1) << std::endl;
		duk_pop(m_DukContext.GetRawContext());
	}

	Script &Script::operator=(Script &&other) {
		if (this == &other)
			return *this;

		m_ListenedToKeys = std::move(other.m_ListenedToKeys);
		m_DukContext     = std::move(other.m_DukContext);
		m_ScriptName     = std::move(other.m_ScriptName);

		auto global{m_DukContext.PushGlobalObject()};
		global.PushPointer("__goPtr", static_cast<void *>(&m_GameObject));

		return *this;
	}

	void Script::Update() {
		duk_get_global_literal(m_DukContext.GetRawContext(), "Update");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop(m_DukContext.GetRawContext());
			return;
		}

		auto const deltaTime{GameTime::GetInstance().GetDeltaTime()};
		duk_push_number(m_DukContext.GetRawContext(), deltaTime);

		if (duk_pcall(m_DukContext.GetRawContext(), 1) != 0)
			std::cerr << "Error: " << duk_safe_to_string(m_DukContext.GetRawContext(), -1) << std::endl;
		duk_pop(m_DukContext.GetRawContext());
	}

	void Script::FixedUpdate() {
		duk_get_global_literal(m_DukContext.GetRawContext(), "FixedUpdate");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop(m_DukContext.GetRawContext());
			return;
		}

		if (duk_pcall(m_DukContext.GetRawContext(), 0) != 0)
			std::cerr << "Error: " << duk_safe_to_string(m_DukContext.GetRawContext(), -1) << std::endl;
		duk_pop(m_DukContext.GetRawContext());
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

	std::filesystem::path const &Script::GetPath() const {
		return m_FilePath;
	}

	void PushPropertyValue(duk_context *ctx, Script::PropertyValue value) {
		if (std::holds_alternative<double>(value)) {
			auto const unwrapped{std::get<double>(value)};

			duk_push_number(ctx, unwrapped);
		} else if (std::holds_alternative<bool>(value)) {
			auto const unwrapped{std::get<bool>(value)};

			duk_push_boolean(ctx, unwrapped);
		} else if (std::holds_alternative<Script::DukUndefined>(value)) {
			duk_push_undefined(ctx);
		} else if (std::holds_alternative<Script::DukNull>(value)) {
			duk_push_null(ctx);
		} else if (std::holds_alternative<std::string>(value)) {
			duk_push_string(ctx, std::get<std::string>(value).c_str());
		} else {
			throw FatalScriptError{"Unhandled type"};
		}
	}

	int SetProperty(duk_context *ctx) {
		auto const numArgs{duk_get_top(ctx)};

		if (numArgs != 2) {
			std::cerr << "setProperty must be passed exactly 2 arguments: the property key and its value" << std::endl;
			return DUK_RET_ERROR;
		}

		std::string const key{duk_require_string(ctx, -2)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Script *ptr{static_cast<Script *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		auto const dukType{duk_get_type(ctx, -2)};

		switch (dukType) {
			case DUK_TYPE_BOOLEAN:
				ptr->SetProperty(key, duk_require_boolean(ctx, -2) != 0);
				break;
			case DUK_TYPE_NUMBER:
				ptr->SetProperty(key, duk_require_number(ctx, -2));
				break;
			case DUK_TYPE_UNDEFINED:
				ptr->SetProperty(key, Script::DukUndefined{});
				break;
			case DUK_TYPE_STRING:
				ptr->SetProperty(key, duk_require_string(ctx, -2));
				break;
			case DUK_TYPE_NULL:
				ptr->SetProperty(key, Script::DukNull{});
				break;
			default:
				std::cerr << "setProperty only supports atomic types" << std::endl;
				break;
		}

		return 0;
	}

	int GetProperty(duk_context *ctx) {
		auto const numArgs{duk_get_top(ctx)};

		if (numArgs != 1) {
			std::cerr << "setProperty must be passed exactly 1 argument: the property key" << std::endl;
			return DUK_RET_ERROR;
		}

		std::string const key{duk_require_string(ctx, -1)};

		duk_push_this(ctx);
		duk_get_prop_literal(ctx, -1, "__ptr");
		Script *ptr{static_cast<Script *>(duk_get_pointer(ctx, -1))};
		duk_pop(ctx);

		auto const value{ptr->GetProperty(key)};

		PushPropertyValue(ctx, value);

		return 1;
	}

	duk_function_list_entry const apiFunctions[]{
	        {"setProperty", SetProperty, 2},
	        {"getProperty", GetProperty, 1},
	        {nullptr, nullptr, 0}
	};

	void Script::ReturnAPIObject(duk_context *ctx) {
		duk_push_object(ctx);

		duk_push_pointer(ctx, static_cast<void *>(this));
		duk_put_prop_literal(ctx, -2, "__ptr");
		duk_put_function_list(ctx, -1, apiFunctions);
	}

	void Script::SetProperty(std::string const &key, PropertyValue value) {
		duk_get_global_literal(m_DukContext.GetRawContext(), "current");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop(m_DukContext.GetRawContext());
			throw FatalScriptError{std::format("Script {}'s current is undefined", GetScriptName())};
		}
		duk_get_prop_literal(m_DukContext.GetRawContext(), -1, "properties");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop_2(m_DukContext.GetRawContext());
			throw FatalScriptError{std::format("Script {}'s current.properties is undefined", GetScriptName())};
		}

		PushPropertyValue(m_DukContext.GetRawContext(), value);

		duk_put_prop_string(m_DukContext.GetRawContext(), -2, key.c_str());
		duk_pop_2(m_DukContext.GetRawContext());
	}

	Script::PropertyValue Script::GetProperty(std::string const &key) const {
		duk_get_global_literal(m_DukContext.GetRawContext(), "current");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop(m_DukContext.GetRawContext());
			throw FatalScriptError{std::format("Script {}'s current is undefined", GetScriptName())};
		}
		duk_get_prop_literal(m_DukContext.GetRawContext(), -1, "properties");
		if (duk_is_undefined(m_DukContext.GetRawContext(), -1)) {
			duk_pop_2(m_DukContext.GetRawContext());
			throw FatalScriptError{std::format("Script {}'s current.properties is undefined", GetScriptName())};
		}

		duk_get_prop_string(m_DukContext.GetRawContext(), -1, key.c_str());

		auto const    dukType{duk_get_type(m_DukContext.GetRawContext(), -1)};
		PropertyValue value{DukUndefined{}};

		switch (dukType) {
			case DUK_TYPE_BOOLEAN:
				value = duk_require_boolean(m_DukContext.GetRawContext(), -1) != 0;
				break;
			case DUK_TYPE_NUMBER:
				value = duk_require_number(m_DukContext.GetRawContext(), -1);
				break;
			case DUK_TYPE_UNDEFINED:
				value = DukUndefined{};
				break;
			case DUK_TYPE_NULL:
				value = DukNull{};
				break;
			case DUK_TYPE_STRING:
				value = duk_require_string(m_DukContext.GetRawContext(), -1);
				break;
			default:
				std::cerr << "getProperty only supports atomic types" << std::endl;
				value = DukNull{};
				break;
		}

		duk_pop_3(m_DukContext.GetRawContext());
		return value;
	}

	void Script::RegisterListenedToKey(InputKeys key, KeyEventType eventType, std::unique_ptr<Command> pCommand) {
		m_ListenedToKeys.emplace_back(key, eventType, std::move(pCommand));
	}

	FatalScriptError::FatalScriptError(std::string errorMessage)
	    : m_ErrorMessage{std::move(errorMessage)} {
	}

	char const *FatalScriptError::what() const noexcept {
		return m_ErrorMessage.c_str();
	}
}// namespace roingine
