#ifndef PLACE_BOMB_COMMAND_H
#define PLACE_BOMB_COMMAND_H

#include <roingine/commands/command.h>
#include <roingine/components/transform.h>

namespace roingine {
	class Transform;
}

namespace bomberman {
	class MovingEntity;

	class PlaceBombCommand final : public roingine::Command {
		roingine::Transform const *m_rpTransform;

	public:
		explicit PlaceBombCommand(roingine::Transform const &rpTransform);

		void Execute() override;
	};
}// namespace bomberman

#endif//PLACE_BOMB_COMMAND_H
