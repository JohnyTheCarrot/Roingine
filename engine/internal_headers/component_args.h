#ifndef COMPONENT_ARGS_H
#define COMPONENT_ARGS_H

#include <roingine/component_init_types.h>
#include <vector>

using duk_context = struct duk_hthread;

namespace roingine {
	class ComponentArgInvalidType final : public std::exception {
	public:
		ComponentArgInvalidType(int argIdx);

		char const *what() const override;

	private:
		std::string m_Message;
	};

	[[nodiscard]]
	std::vector<ComponentInitArgument> CollectComponentArgs(duk_context *);

	void PushComponentArgsToDuk(std::vector<ComponentInitArgument> const &args, duk_context *ctx);
}// namespace roingine

#endif// COMPONENT_ARGS_H
