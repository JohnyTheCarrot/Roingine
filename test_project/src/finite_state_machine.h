#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include <memory>

namespace bomberman {
	class FiniteStateMachine {
	public:
		virtual void OnEnter() = 0;

		virtual void OnExit() = 0;

		[[nodiscard]]
		virtual std::unique_ptr<FiniteStateMachine> Update() = 0;

		FiniteStateMachine() = default;

		virtual ~FiniteStateMachine() = default;
	};
};// namespace bomberman

#endif//FINITE_STATE_MACHINE_H
