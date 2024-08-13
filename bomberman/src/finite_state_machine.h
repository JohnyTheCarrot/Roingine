#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include <memory>

namespace bomberman {
	template<typename Input>
	class FiniteStateMachine {
	public:
		virtual void OnEnter(){};

		virtual void OnExit(){};

		[[nodiscard]]
		virtual std::unique_ptr<FiniteStateMachine> Update(Input const &input) = 0;

		FiniteStateMachine() = default;

		virtual ~FiniteStateMachine() = default;
	};

	template<typename Input>
	class NullFSM final : public FiniteStateMachine<Input> {
	public:
		std::unique_ptr<FiniteStateMachine<Input>> Update(Input const &) override {
			return nullptr;
		}
	};
};// namespace bomberman

#endif//FINITE_STATE_MACHINE_H
