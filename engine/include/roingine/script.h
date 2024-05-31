#ifndef SCRIPT_H
#define SCRIPT_H

#include <filesystem>
#include <roingine/commands/registered_keyboard_command.h>
#include <roingine/components/component.h>
#include <roingine/duk_wrappers.h>
#include <roingine/gameobject.h>
#include <roingine/input.h>
#include <string_view>
#include <variant>
#include <vector>

namespace roingine {
	class Scripts;

	class Script final {
	public:
		struct DukUndefined final {};

		struct DukNull final {};

		using DukValue = std::variant<double, std::string, bool, DukUndefined, DukNull>;

		using CppFunctionCaller = std::function<DukValue(std::string_view, GameObject, std::vector<DukValue> &&)>;

		Script(Scripts &scriptsComponent, std::string_view fileName, std::optional<CppFunctionCaller> const &caller);

		~Script();

		Script(Script &&);

		Script &operator=(Script &&);

		void Update();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		std::string_view GetScriptName() const;

		[[nodiscard]]
		std::filesystem::path const &GetPath() const;

		void ReturnAPIObject(duk_context *ctx);

		[[nodiscard]]
		DukContext &GetDukContext();

		void SetProperty(std::string const &key, DukValue value);

		[[nodiscard]]
		static DukValue FromDukToValue(duk_context *ctx, int index = -1);

		static void PushToDukFromValue(duk_context *ctx, DukValue const &value);

		[[nodiscard]]
		DukValue GetProperty(std::string const &key) const;

		[[nodiscard]]
		DukValue CallMethod(std::string const &key, std::vector<DukValue> const &arguments) const;

		void RegisterListenedToKey(InputKeys key, KeyEventType eventType, std::unique_ptr<roingine::Command> pCommand);

		[[nodiscard]]
		GameObject GetGameObject() const noexcept;

		void SetGameObjectScene(Scene *pScene) noexcept;

		void SetCppFunctionCaller(CppFunctionCaller const &caller);

		[[nodiscard]]
		Script::DukValue
		CallCppFunction(std::string_view name, GameObject gameObject, std::vector<DukValue> &&arguments);

	private:
		void CallJsFunctionByName(std::string_view name);

		[[nodiscard]]
		GameObject *GetGameObjectPtr() const noexcept;

		std::filesystem::path                  m_FilePath;
		std::reference_wrapper<Scripts>        m_ScriptsComponent;
		std::vector<RegisteredKeyboardCommand> m_ListenedToKeys;
		DukContext                             m_DukContext;
		std::string                            m_ScriptName;

		std::optional<CppFunctionCaller> m_CppFunctionCaller{};
	};
}// namespace roingine

#endif// SCRIPT_H
