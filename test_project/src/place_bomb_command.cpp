#include "place_bomb_command.h"

#include "event_queue.h"

namespace bomberman {
	PlaceBombCommand::PlaceBombCommand(roingine::Transform const &rpTransform)
	    : m_rpTransform{&rpTransform} {
	}

	void PlaceBombCommand::Execute() {
		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::BombPlaceRequest>(
		        m_rpTransform->GetWorldPosition()
		);
	}
}// namespace bomberman
