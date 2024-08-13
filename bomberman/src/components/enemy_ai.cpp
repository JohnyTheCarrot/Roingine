#include "enemy_ai.h"

#include "enemy.h"

#include <glm/ext/matrix_transform.hpp>
#include <random>
#include <roingine/game_time.h>

namespace bomberman {
	constexpr glm::vec2 c_UpVec{0, 1};
	constexpr glm::vec2 c_DownVec{0, -1};
	constexpr glm::vec2 c_LeftVec{-1, 0};
	constexpr glm::vec2 c_RightVec{1, 0};

	EnemyWandering::EnemyWandering(Enemy &enemy, LivingEntity &livingEntity, Axis axis, Direction direction)
	    : m_rpEnemy{&enemy}
	    , m_rpLivingEntity{&livingEntity}
	    , m_Axis{axis}
	    , m_Direction{direction} {
	}

	std::unique_ptr<FiniteStateMachine<EnemyFSMInput>> EnemyWandering::Update(EnemyFSMInput const &) {
		constexpr double c_ChanceToChangeDirection{0.1};

		static std::random_device          rd;
		static std::mt19937                gen(rd());
		static std::bernoulli_distribution dist{c_ChanceToChangeDirection};
		m_TimeSinceDirChangeCheck += roingine::GameTime::GetInstance().GetDeltaTime();

		if (m_TimeSinceDirChangeCheck >= c_TimeBetweenDirChangeChecks && !m_PlansToChangeDir) {
			m_TimeSinceDirChangeCheck = 0.f;
			m_PlansToChangeDir        = dist(gen);
		}

		if (m_PlansToChangeDir && m_rpEnemy->IsInOneTile()) {
			auto const [up, down, left, right]{m_rpEnemy->GetFreeDirections()};

			if (m_Axis == Axis::Y) {
				if (left) {
					m_Axis      = Axis::X;
					m_Direction = Direction::Negative;
				}

				if (right) {
					m_Axis      = Axis::X;
					m_Direction = Direction::Positive;
				}
			} else {
				if (up) {
					m_Axis      = Axis::Y;
					m_Direction = Direction::Positive;
				}

				if (down) {
					m_Axis      = Axis::Y;
					m_Direction = Direction::Negative;
				}
			}
			m_PlansToChangeDir = false;
		}

		auto const instruction{[this] {
			switch (m_Axis) {
				case Axis::X:
					return m_Direction == Direction::Positive ? MoveInstruction::Right : MoveInstruction::Left;
				default:
					return m_Direction == Direction::Positive ? MoveInstruction::Up : MoveInstruction::Down;
			}
		}()};

		m_rpLivingEntity->Instruct(instruction);
		if (!m_rpEnemy->IsInOneTile())
			return nullptr;

		auto const freeDirections{m_rpEnemy->GetFreeDirections()};

		if (m_Axis == Axis::X) {
			if (m_Direction == Direction::Negative && !freeDirections.left)
				m_Direction = Direction::Positive;
			else if (m_Direction == Direction::Positive && !freeDirections.right)
				m_Direction = Direction::Negative;
		} else {
			if (m_Direction == Direction::Negative && !freeDirections.down)
				m_Direction = Direction::Positive;
			else if (m_Direction == Direction::Positive && !freeDirections.up)
				m_Direction = Direction::Negative;
		}

		return nullptr;
	}

	EnemyAi::EnemyAi(roingine::GameObject gameObject, AIType aiType)
	    : Component{gameObject}
	    , m_rpEnemy{&gameObject.GetComponent<Enemy>()}
	    , m_rpLivingEntity{&gameObject.GetComponent<LivingEntity>()}
	    , m_AIType{aiType}
	    , m_pCurrentState{std::make_unique<EnemyWandering>(
	              *m_rpEnemy, *m_rpLivingEntity, EnemyWandering::Axis::X, EnemyWandering::Direction::Positive
	      )} {
	}

	void EnemyAi::Update() {
		if (auto newState{m_pCurrentState->Update(EnemyFSMInput{})}; newState != nullptr) {
			m_pCurrentState->OnExit();
			m_pCurrentState = std::move(newState);
			m_pCurrentState->OnEnter();
		}
	}
}// namespace bomberman
