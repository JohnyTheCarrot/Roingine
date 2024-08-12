#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "components/living_entity.h"

#include <roingine/commands/command.h>

namespace bomberman {
	class LivingEntity;

	class LivingEntityCommand final : public roingine::Command {
		LivingEntity           *m_rpLivingEntityComponent;
		LivingEntityInstruction m_Instruction;

	public:
		LivingEntityCommand(LivingEntity &rpLivingEntityComponent, LivingEntityInstruction const &instruction);

		void Execute() const override;
	};
}// namespace bomberman

#endif//MOVE_COMMAND_H
