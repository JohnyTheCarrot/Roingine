#ifndef LIVING_ENTITY_H
#define LIVING_ENTITY_H

#include "../event_queue.h"
#include "../finite_state_machine.h"

#include <roingine/components/component.h>
#include <roingine/event_queue.h>
#include <roingine/gameobject.h>

namespace bomberman {
	enum class MoveInstruction {
		Up,
		Down,
		Left,
		Right,
		Idle,
	};

	enum class Action { Primary, Secondary };

	struct DieInstruction final {};

	using LivingEntityInstruction = std::variant<MoveInstruction, DieInstruction, Action>;

	class LivingEntity final : public roingine::Component {
	public:
		using FSMNodePtr = std::unique_ptr<FiniteStateMachine<LivingEntityInstruction>>;

		LivingEntity(roingine::GameObject gameObject, FSMNodePtr &&pInitialState);

		void Instruct(LivingEntityInstruction const &input);

		[[nodiscard]]
		bool GetInvulnerable() const;

		void SetInvulnerable(bool isInvulnerable);

	private:
		void OnCaughtInExplosion(event_queue::ExplosionData const &data);

		roingine::EventHandlerHandle<event_queue::EventQueue> m_hExplosionHandler;
		FSMNodePtr                                            m_pCurrentState;
		bool                                                  m_IsInvulnerable{false};
	};
}// namespace bomberman

#endif//LIVING_ENTITY_H
