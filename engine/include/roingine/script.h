#ifndef SCRIPT_H
#define SCRIPT_H

#include <roingine/commands/registered_keyboard_command.h>
#include <roingine/components/component.h>
#include <roingine/duk_wrappers.h>
#include <roingine/gameobject.h>
#include <roingine/input.h>
#include <string_view>
#include <vector>

namespace roingine {
	class Script final {
	public:
		Script(GameObject gameObject, std::string_view fileName);

		~Script();

		Script(Script &&);

		Script &operator=(Script &&);

		void Update();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		std::string_view GetScriptName() const;

		void RegisterListenedToKey(InputKeys key, KeyEventType eventType, std::unique_ptr<roingine::Command> pCommand);

	private:
		void CallJsFunctionByName(std::string_view name);

		GameObject                             m_GameObject;
		std::vector<RegisteredKeyboardCommand> m_ListenedToKeys;
		DukContext                             m_DukContext;
		std::string                            m_ScriptName;
	};
}// namespace roingine

#endif// SCRIPT_H
