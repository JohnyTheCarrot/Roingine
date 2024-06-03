#ifndef COMPONENT_INIT_TYPES_H
#define COMPONENT_INIT_TYPES_H

#include <exception>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace roingine {
	class ComponentArgumentIncorrectType final : public std::exception {
	public:
		ComponentArgumentIncorrectType(
		        std::size_t argIdx, std::string_view expectedTypeName, std::string_view actualTypeName
		);

		char const *what() const override;

	private:
		std::string m_Message;
	};

	using ComponentInitArgument = std::variant<std::string, double, bool>;

	namespace comp_init {
		[[nodiscard]]
		std::string_view GetTypeName(ComponentInitArgument const &);

		[[nodiscard]]
		std::string RequireString(std::size_t argIdx, std::vector<ComponentInitArgument> const &args);

		[[nodiscard]]
		double RequireDouble(std::size_t argIdx, std::vector<ComponentInitArgument> const &args);

		[[nodiscard]]
		bool RequireBool(std::size_t argIdx, std::vector<ComponentInitArgument> const &args);
	}// namespace comp_init
}// namespace roingine

#endif// COMPONENT_INIT_TYPES_H
