#include "place_bomb_command.h"

#include "components/player.h"
#include "event_queue.h"

namespace bomberman {
	PlaceBombCommand::PlaceBombCommand(roingine::GameObject bomber)
	    : m_Bomber{bomber} {
	}

	void PlaceBombCommand::Execute() const {
		auto const &transform{m_Bomber.GetComponent<roingine::Transform>()};
		auto const *rpPlayer{m_Bomber.GetOptionalComponent<Player>()};
		if (rpPlayer == nullptr)
			return;

		auto &playerInfo{rpPlayer->GetPlayerInfo()};

		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::BombPlaceRequest>(
		        &playerInfo, transform.GetWorldPosition()
		);
	}
}// namespace bomberman
