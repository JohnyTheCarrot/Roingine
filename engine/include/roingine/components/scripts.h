#ifndef SCRIPTS_H
#define SCRIPTS_H

#include <memory>
#include <roingine/components/component.h>
#include <roingine/input.h>
#include <roingine/script.h>
#include <roingine/unique_duk_context.h>
#include <string_view>
#include <unordered_map>

namespace roingine {
	class Scripts final : public roingine::Component {
	public:
		Scripts(GameObject &gameObject);

		void Update() override;

		void FixedUpdate() override;

		void Render() const override;

		static constexpr char const *NAME{"Scripts"};

		[[nodiscard]]
		char const *GetName() const override;

		[[nodiscard]]
		duk_function_list_entry const *SetUpScriptAPI(duk_context *ctx) const override;

		[[nodiscard]]
		static std::unique_ptr<Scripts> JSFactory(GameObject *, duk_context *);

		void AddScript(std::string_view fileName);

		[[nodiscard]]
		Script *GetScript(std::string const &name);

		void ExecuteOnEveryScript(std::function<void(DukContext &)>const&fn);

	private:
		std::unordered_map<std::string, Script> m_Scripts;
	};
}// namespace roingine

#endif// SCRIPTS_H
