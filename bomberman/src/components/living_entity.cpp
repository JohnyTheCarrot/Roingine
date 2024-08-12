#include "living_entity.h"

#include <iostream>
#include <roingine/components/transform.h>

namespace bomberman {
	LivingEntity::LivingEntity(roingine::GameObject gameObject, FSMNodePtr &&pInitialState)
	    : Component{gameObject}
	    , m_hExplosionHandler{event_queue::EventQueue::GetInstance()
	                                  .AttachEventHandler<event_queue::EventType::Explosion>(
	                                          [this](event_queue::ExplosionData const &data) {
		                                          auto const &transform{
		                                                  GetGameObject().GetComponent<roingine::Transform>()
		                                          };
		                                          auto const &pos{transform.GetWorldPosition()};

		                                          if (data.IsInExplosion(pos)) {
			                                          std::cout << "dieded" << std::endl;
		                                          }
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
}// namespace bomberman
