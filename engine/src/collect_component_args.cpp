#include "collect_component_args.h"
#include <duktape.h>
#include <format>

namespace roingine {
	std::vector<ComponentInitArgument> CollectComponentArgs(duk_context *ctx) {
		auto const                         nArgs{duk_get_top(ctx) - 1};
		std::vector<ComponentInitArgument> collectedArgs{};
		collectedArgs.reserve(nArgs);

		for (int idx{1}; idx < duk_get_top_index(ctx); ++idx) {
			auto const type{duk_get_type(ctx, idx)};

			switch (type) {
				case DUK_TYPE_BOOLEAN:
					collectedArgs.push_back(duk_require_boolean(ctx, idx) != 0);
					break;
				case DUK_TYPE_STRING:
					collectedArgs.push_back(duk_require_string(ctx, idx));
					break;
				case DUK_TYPE_NUMBER:
					collectedArgs.push_back(duk_require_number(ctx, idx));
					break;
				default:
					throw ComponentArgInvalidType{idx};
			}
		}

		return collectedArgs;
	}

	ComponentArgInvalidType::ComponentArgInvalidType(int argIdx)
	    : m_Message{std::format("Argument type at {} is invalid.", argIdx)} {
	}

	char const *ComponentArgInvalidType::what() const {
		return m_Message.c_str();
	}
}// namespace roingine
