#ifndef SCRIPT_H
#define SCRIPT_H

#include <exception>
#include <roingine/components/component.h>
#include <roingine/gameobject.h>
#include <roingine/input.h>
#include <roingine/unique_duk_context.h>
#include <string_view>
#include <vector>

namespace roingine {
	class ScriptCompilationFailedException final : public std::exception {
	public:
		ScriptCompilationFailedException(std::string errorMessage);

		char const *what() const noexcept override;

	private:
		std::string m_ErrorMessage;
	};

	class FatalScriptError final : public std::exception {
	public:
		FatalScriptError(std::string errorMessage);

		char const *what() const noexcept override;

	private:
		std::string m_ErrorMessage;
	};

	class Script final {
	public:
		Script(GameObject gameObject, std::string_view fileName);

		Script(Script &&);

		Script &operator=(Script &&);

		void Update();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		std::string_view GetScriptName() const;

	private:
		void CallJsFunctionByName(std::string_view name);

		GameObject             m_GameObject;
		std::vector<InputKeys> m_ListenedToKeys{};
		UniqueDukContext       m_DukContext;
		std::string            m_ScriptName;
	};
}// namespace roingine

#endif// SCRIPT_H
