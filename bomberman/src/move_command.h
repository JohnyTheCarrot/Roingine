#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <glm/vec2.hpp>
#include <roingine/commands/command.h>

namespace bomberman {
	class MovingEntity;

	class MoveCommand final : public roingine::Command {
		glm::vec2     m_Direction;
		MovingEntity *m_rpMovingEntityComponent;

	public:
		MoveCommand(MovingEntity *rpMovingEntityComponent, glm::vec2 direction);

		void Execute() const override;
	};
}// namespace bomberman

#endif//MOVE_COMMAND_H
