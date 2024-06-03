#ifndef COLLECT_COMPONENT_ARGS_H
#define COLLECT_COMPONENT_ARGS_H

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
}// namespace roingine

#endif// COLLECT_COMPONENT_ARGS_H
