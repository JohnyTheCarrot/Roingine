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
	class Script final {
	public:
		Script(GameObject &gameObject, std::string_view fileName);

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

		struct DukUndefined final {};

		struct DukNull final {};

		using DukValue = std::variant<double, std::string, bool, DukUndefined, DukNull>;

		void SetProperty(std::string const &key, DukValue value);

		[[nodiscard]]
		static DukValue FromDukToValue(duk_context *ctx, int index = -1);

		static void PushToDukFromValue(duk_context *ctx, DukValue const &value);

		[[nodiscard]]
		DukValue GetProperty(std::string const &key) const;

		[[nodiscard]]
		DukValue CallMethod(std::string const &key, std::vector<DukValue> const &arguments) const;

		void RegisterListenedToKey(InputKeys key, KeyEventType eventType, std::unique_ptr<roingine::Command> pCommand);

	private:
		void CallJsFunctionByName(std::string_view name);

		std::filesystem::path                  m_FilePath;
		std::reference_wrapper<GameObject>     m_GameObject;
		std::vector<RegisteredKeyboardCommand> m_ListenedToKeys;
		DukContext                             m_DukContext;
		std::string                            m_ScriptName;
	};
}// namespace roingine

#endif// SCRIPT_H
