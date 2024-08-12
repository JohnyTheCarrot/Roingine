#include "living_entity_command.h"
#include "components/living_entity.h"

namespace bomberman {
	LivingEntityCommand::LivingEntityCommand(
	        LivingEntity &rpLivingEntityComponent, LivingEntityInstruction const &instruction
	)
	    : m_rpLivingEntityComponent{&rpLivingEntityComponent}
	    , m_Instruction{instruction} {
	}

	void LivingEntityCommand::Execute() const {
		m_rpLivingEntityComponent->Instruct(m_Instruction);
	}
}// namespace bomberman
