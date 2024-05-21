#include <duktape.h>
#include <roingine/unique_duk_context.h>

namespace roingine {
	void DukContextDestroyer::operator()(duk_context *dukContext) {
		duk_destroy_heap(dukContext);
	}
}// namespace roingine
