#include "move_command.h"
#include "components/moving_entity.h"

namespace bomberman {
	MoveCommand::MoveCommand(MovingEntity *rpMovingEntityComponent, glm::vec2 direction)
	    : m_Direction{direction}
	    , m_rpMovingEntityComponent{rpMovingEntityComponent} {
	}

	void MoveCommand::Execute() const {
		m_rpMovingEntityComponent->Move(m_Direction);
	}
}// namespace bomberman
