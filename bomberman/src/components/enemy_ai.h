#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include "../finite_state_machine.h"
#include "enemy.h"

#include <roingine/components/component.h>
#include <roingine/gameobject.h>

namespace bomberman {
	class LivingEntity;

	struct EnemyFSMInput final {};

	class EnemyWandering final : public FiniteStateMachine<EnemyFSMInput> {
	public:
		enum class Axis { X, Y };
		enum class Direction { Positive, Negative };

		EnemyWandering(Enemy &enemy, LivingEntity &livingEntity, Axis axis, Direction direction);

		[[nodiscard]]
		std::unique_ptr<FiniteStateMachine> Update(EnemyFSMInput const &input) override;

	private:
		Enemy        *m_rpEnemy;
		LivingEntity *m_rpLivingEntity;
		Axis          m_Axis;
		Direction     m_Direction;
		float         m_TimeSinceDirChangeCheck{0.f};
		bool          m_PlansToChangeDir{false};

		static constexpr float c_TimeBetweenDirChangeChecks{0.5f};
	};

	class EnemyAi final : public roingine::Component {
	public:
		enum class AIType {
			Simple,    // Moves in a straight line, low chance of changing direction
			Aggressive,// Moves in a straight line, high chance of changing direction
		};

		EnemyAi(roingine::GameObject gameObject, AIType aiType);

		void Update() override;

	private:
		Enemy                                             *m_rpEnemy;
		LivingEntity                                      *m_rpLivingEntity;
		AIType                                             m_AIType;
		std::unique_ptr<FiniteStateMachine<EnemyFSMInput>> m_pCurrentState;
	};
}// namespace bomberman

#endif//ENEMY_AI_H
