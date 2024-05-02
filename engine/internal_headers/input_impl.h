#ifndef INPUT_INTERNAL_H
#define INPUT_INTERNAL_H

#include <roingine/input.h>

namespace roingine {
	class Input::Impl final {
	public:
		[[nodiscard]]
		bool ProcessInput();

		[[nodiscard]]
		bool IsPressingAssignmentButton() const;
	};
}// namespace roingine

#endif// INPUT_INTERNAL_H
