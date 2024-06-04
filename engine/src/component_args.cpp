#include "component_args.h"
#include <duktape.h>
#include <format>
#include <iostream>

namespace roingine {
	std::vector<JSData> CollectDataFromDukArgs(duk_context *ctx) {
		auto const                         nArgs{duk_get_top(ctx) - 1};
		std::vector<JSData> collectedArgs{};
		collectedArgs.reserve(nArgs);

		for (int idx{1}; idx <= duk_get_top_index(ctx); ++idx) {
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
					throw JSDataInvalidType{idx};
			}
		}

		return collectedArgs;
	}

	JSDataInvalidType::JSDataInvalidType(int argIdx)
	    : m_Message{std::format("Data type at {} is invalid.", argIdx)} {
	}

	char const *JSDataInvalidType::what() const {
		return m_Message.c_str();
	}

	void PushDataToDuk(std::vector<JSData> const &args, duk_context *ctx) {
		for (auto const &arg: args) {
			if (std::holds_alternative<std::string>(arg))
				duk_push_string(ctx, std::get<std::string>(arg).c_str());
			else if (std::holds_alternative<double>(arg))
				duk_push_number(ctx, std::get<double>(arg));
			else if (std::holds_alternative<bool>(arg))
				duk_push_boolean(ctx, std::get<bool>(arg));
		}
	}
}// namespace roingine
