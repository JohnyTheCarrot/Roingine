#ifndef SCRIPT_H
#define SCRIPT_H

#include <exception>
#include <roingine/components/component.h>
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

	class Script final : public roingine::Component {
	public:
		Script(GameObject &gameObject, std::string_view fileName);

		void Update() override;
		void FixedUpdate() override;
		void Render() const override;

	private:
		std::vector<InputKeys> m_ListenedToKeys{};
		UniqueDukContext m_DukContext;
	};
}// namespace roingine

#endif// SCRIPT_H
