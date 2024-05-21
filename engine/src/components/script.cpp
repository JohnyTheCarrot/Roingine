#include <duktape.h>
#include <format>
#include <fstream>
#include <iostream>
#include <roingine/commands/command.h>
#include <roingine/components/script.h>
#include <roingine/game_time.h>
#include <roingine/input.h>
#include <roingine/roingine.h>

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

		std::cout << std::endl;

		return 0;
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

	duk_function_list_entry const roingineFunctions[]{{"print", Print, DUK_VARARGS}, {nullptr, nullptr, 0}};

	duk_function_list_entry const inputFunctions[]{
	        {"onKeyDown", OnKeyDown, 2},
	        {"onKeyUp", OnKeyUp, 2},
	        {"onKeyHeld", OnKeyHeld, 2},
	        {"onKeyLongPressed", OnKeyLongPress, 2},
	        {nullptr, nullptr, 0}
	};

	duk_number_list_entry const inputNumberConstants[]{
	        {"KEY_A", static_cast<int>(InputKeys::A)},
	        {"KEY_B", static_cast<int>(InputKeys::B)},
	        {"KEY_C", static_cast<int>(InputKeys::C)},
	        {nullptr, 0}
	};

	Script::Script(GameObject &gameObject, std::string_view fileName)
	    : Component{gameObject}
	    , m_DukContext{duk_create_heap_default()} {
		std::ifstream ifstream{fileName.data()};
		if (!ifstream)
			throw ScriptCompilationFailedException{"File not found"};

		duk_push_global_object(m_DukContext.get());
		duk_push_object(m_DukContext.get());
		duk_put_function_list(m_DukContext.get(), -1, roingineFunctions);
		duk_put_prop_string(m_DukContext.get(), -2, "roingine");

		duk_push_object(m_DukContext.get());
		duk_put_number_list(m_DukContext.get(), -1, inputNumberConstants);
		duk_put_function_list(m_DukContext.get(), -1, inputFunctions);
		duk_put_prop_string(m_DukContext.get(), -2, "input");
		duk_pop(m_DukContext.get());

		std::string inputCode{std::istreambuf_iterator<char>(ifstream), {}};

		if (duk_peval_string(m_DukContext.get(), inputCode.c_str()) != 0) {
			auto const errMessage{duk_safe_to_string(m_DukContext.get(), -1)};
			throw ScriptCompilationFailedException{errMessage};
		}
	}

	void Script::Update() {
		duk_get_global_literal(m_DukContext.get(), "Update");
		if (duk_is_undefined(m_DukContext.get(), -1)) {
			duk_pop(m_DukContext.get());
			return;
		}

		duk_push_number(m_DukContext.get(), static_cast<duk_double_t>(GameTime::GetInstance().GetDeltaTime()));
		if (duk_pcall(m_DukContext.get(), 1) != 0)
			std::cerr << "Error: " << duk_safe_to_string(m_DukContext.get(), -1) << std::endl;
		duk_pop(m_DukContext.get());
	}

	void Script::FixedUpdate() {
	}

	void Script::Render() const {
	}

	ScriptCompilationFailedException::ScriptCompilationFailedException(std::string errorMessage)
	    : m_ErrorMessage{std::move(errorMessage)} {
	}

	char const *ScriptCompilationFailedException::what() const noexcept {
		return m_ErrorMessage.c_str();
	}
}// namespace roingine
