#include <format>
#include <roingine/component_init_types.h>

namespace roingine {
	ComponentArgumentIncorrectType::ComponentArgumentIncorrectType(
	        std::size_t argIdx, std::string_view expectedTypeName, std::string_view actualTypeName
	)
	    : m_Message{std::format(
	              "Expected component argument type {} at {} but got {}", expectedTypeName, argIdx, actualTypeName
	      )} {
	}

	char const *ComponentArgumentIncorrectType::what() const {
		return m_Message.c_str();
	}

	namespace comp_init {
		std::string_view GetTypeName(ComponentInitArgument const &arg) {
			if (std::holds_alternative<std::string>(arg))
				return "string";

			if (std::holds_alternative<double>(arg))
				return "double";

			if (std::holds_alternative<bool>(arg))
				return "bool";

			return "unknown type";
		}

		std::string RequireString(std::size_t argIdx, std::vector<ComponentInitArgument> const &args) {
			auto const arg{args.at(argIdx)};

			if (std::holds_alternative<std::string>(arg))
				return std::get<std::string>(arg);

			throw ComponentArgumentIncorrectType{argIdx, "string", GetTypeName(arg)};
		}

		double RequireDouble(std::size_t argIdx, std::vector<ComponentInitArgument> const &args) {
			auto const arg{args.at(argIdx)};

			if (std::holds_alternative<double>(arg))
				return std::get<double>(arg);

			throw ComponentArgumentIncorrectType{argIdx, "double", GetTypeName(arg)};
		}

		bool RequireBool(std::size_t argIdx, std::vector<ComponentInitArgument> const &args) {
			auto const arg{args.at(argIdx)};

			if (std::holds_alternative<bool>(arg))
				return std::get<bool>(arg);

			throw ComponentArgumentIncorrectType{argIdx, "bool", GetTypeName(arg)};
		}
	}// namespace comp_init
}// namespace roingine

// namespace roingine
