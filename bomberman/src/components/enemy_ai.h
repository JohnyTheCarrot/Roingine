#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include "../finite_state_machine.h"
#include "enemy.h"

#include <roingine/components/component.h>
#include <roingine/gameobject.h>

namespace bomberman {
	class EnemyWandering final : public FiniteStateMachine {
	public:
		enum class Axis { X, Y };
		enum class Direction { Positive, Negative };

		void OnEnter() override;

		void OnExit() override;

		std::unique_ptr<FiniteStateMachine> Update() override;

		EnemyWandering(Enemy &enemy, Axis axis, Direction direction);

	private:
		Enemy    *m_rpEnemy;
		Axis      m_Axis;
		Direction m_Direction;
		float     m_TimeSinceDirChangeCheck{0.f};
		bool      m_PlansToChangeDir{false};

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
		Enemy                              *m_rpEnemy;
		AIType                              m_AIType;
		std::unique_ptr<FiniteStateMachine> m_pCurrentState;
	};
}// namespace bomberman

#endif//ENEMY_AI_H
