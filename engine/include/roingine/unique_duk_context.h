#ifndef UNIQUE_DUK_CONTEXT_H
#define UNIQUE_DUK_CONTEXT_H

#include <memory>

using duk_context = struct duk_hthread;

namespace roingine {
	class DukContextDestroyer final {
	public:
		void operator()(duk_context *);
	};

	using UniqueDukContext = std::unique_ptr<duk_context, DukContextDestroyer>;
}// namespace roingine

#endif// UNIQUE_DUK_CONTEXT_H
