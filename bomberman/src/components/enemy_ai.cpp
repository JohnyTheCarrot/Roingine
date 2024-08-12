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

	void EnemyWandering::OnEnter() {
		m_rpEnemy->FlipDirection();
	}

	void EnemyWandering::OnExit() {
	}

	// void EnemyWandering::Update() {
	// constexpr double c_ChanceToChangeDirection{0.1};
	//
	// static std::random_device          rd;
	// static std::mt19937                gen(rd());
	// static std::bernoulli_distribution dist{c_ChanceToChangeDirection};
	// m_TimeSinceDirChangeCheck += roingine::GameTime::GetInstance().GetDeltaTime();
	//
	// if (m_TimeSinceDirChangeCheck >= c_TimeBetweenDirChangeChecks && !m_PlansToChangeDir) {
	// 	m_TimeSinceDirChangeCheck = 0.f;
	// 	m_PlansToChangeDir        = dist(gen);
	// }
	//
	// if (m_PlansToChangeDir && m_rpEnemy->IsInOneTile()) {
	// 	auto const [up, down, left, right]{m_rpEnemy->GetFreeDirection()};
	//
	// 	if (m_Axis == Axis::Y) {
	// 		if (left)
	// 			return std::make_unique<EnemyWandering>(*m_rpEnemy, Axis::X, Direction::Negative);
	//
	// 		if (right)
	// 			return std::make_unique<EnemyWandering>(*m_rpEnemy, Axis::X, Direction::Positive);
	// 	} else {
	// 		if (up)
	// 			return std::make_unique<EnemyWandering>(*m_rpEnemy, Axis::Y, Direction::Positive);
	//
	// 		if (down)
	// 			return std::make_unique<EnemyWandering>(*m_rpEnemy, Axis::Y, Direction::Negative);
	// 	}
	// 	m_PlansToChangeDir = false;
	// }
	//
	// auto const vec{[this] {
	// 	switch (m_Axis) {
	// 		case Axis::X:
	// 			return m_Direction == Direction::Positive ? c_RightVec : c_LeftVec;
	// 		default:
	// 			return m_Direction == Direction::Positive ? c_UpVec : c_DownVec;
	// 	}
	// }()};
	//
	// if (m_rpEnemy->Move(vec))
	// 	return nullptr;
	//
	// return std::make_unique<EnemyWandering>(
	//         *m_rpEnemy, m_Axis, m_Direction == Direction::Positive ? Direction::Negative : Direction::Positive
	// );
	// }

	EnemyWandering::EnemyWandering(Enemy &enemy, Axis axis, Direction direction)
	    : m_rpEnemy{&enemy}
	    , m_Axis{axis}
	    , m_Direction{direction} {
	}

	std::unique_ptr<FiniteStateMachine<EnemyFSMInput>> EnemyWandering::Update(EnemyFSMInput const &) {
		return nullptr;
	}

	EnemyAi::EnemyAi(roingine::GameObject gameObject, AIType aiType)
	    : Component{gameObject}
	    , m_rpEnemy{&gameObject.GetComponent<Enemy>()}
	    , m_AIType{aiType}
	    , m_pCurrentState{std::make_unique<EnemyWandering>(
	              *m_rpEnemy, EnemyWandering::Axis::X, EnemyWandering::Direction::Positive
	      )} {
	}

	void EnemyAi::Update() {
		// if (auto newState{m_pCurrentState->Update(EnemyFSMInput{})}; newState != nullptr) {
		// 	m_pCurrentState->OnExit();
		// 	m_pCurrentState = std::move(newState);
		// 	m_pCurrentState->OnEnter();
		// }
	}
}// namespace bomberman
