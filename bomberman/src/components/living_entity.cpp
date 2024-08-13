#include "living_entity.h"

#include "../player_info.h"
#include "enemy.h"
#include "player.h"

#include <roingine/components/transform.h>

namespace bomberman {
	LivingEntity::LivingEntity(roingine::GameObject gameObject, FSMNodePtr &&pInitialState)
	    : Component{gameObject}
	    , m_hExplosionHandler{event_queue::EventQueue::GetInstance()
	                                  .AttachEventHandler<event_queue::EventType::Explosion>(
	                                          [this](event_queue::ExplosionData const &data) {
		                                          OnCaughtInExplosion(data);
	                                          }
	                                  )}
	    , m_pCurrentState{std::move(pInitialState)} {
		m_pCurrentState->OnEnter();
	}

	void LivingEntity::Instruct(LivingEntityInstruction const &input) {
		if (auto newNode{m_pCurrentState->Update(input)}; newNode != nullptr) {
			m_pCurrentState->OnExit();
			m_pCurrentState = std::move(newNode);
			m_pCurrentState->OnEnter();
		}
	}

	bool LivingEntity::GetInvulnerable() const {
		return m_IsInvulnerable;
	}

	void LivingEntity::SetInvulnerable(bool isInvulnerable) {
		m_IsInvulnerable = isInvulnerable;
	}

	void LivingEntity::OnCaughtInExplosion(event_queue::ExplosionData const &data) {
		if (m_IsInvulnerable)
			return;

		auto const &transform{GetGameObject().GetComponent<roingine::Transform>()};

		if (auto const &pos{transform.GetWorldPosition()}; !data.IsInExplosion(pos))
			return;

		if (auto *rpPlayer{GetGameObject().GetOptionalComponent<Player>()}; rpPlayer != nullptr) {
			rpPlayer->Hurt();
			return;
		}

		if (auto *rpEnemy{GetGameObject().GetOptionalComponent<Enemy>()}; rpEnemy != nullptr) {
			rpEnemy->Kill();
		}
	}
}// namespace bomberman
