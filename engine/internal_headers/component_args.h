#ifndef COMPONENT_ARGS_H
#define COMPONENT_ARGS_H

#include <roingine/component_init_types.h>
#include <vector>

using duk_context = struct duk_hthread;

namespace roingine {
	class JSDataInvalidType final : public std::exception {
	public:
		JSDataInvalidType(int argIdx);

		char const *what() const override;

	private:
		std::string m_Message;
	};

	[[nodiscard]]
	std::vector<JSData> CollectDataFromDukArgs(duk_context *);

	void PushDataToDuk(std::vector<JSData> const &args, duk_context *ctx);
}// namespace roingine

#endif// COMPONENT_ARGS_H
