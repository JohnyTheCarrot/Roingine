#ifndef INPUT_INTERNAL_H
#define INPUT_INTERNAL_H

#include "input.h"

namespace roingine {
	class Input::Impl final {
	public:
		[[nodiscard]]
		bool ProcessInput();
	};
}// namespace roingine

#endif// INPUT_INTERNAL_H
