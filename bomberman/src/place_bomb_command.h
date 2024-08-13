#ifndef PLACE_BOMB_COMMAND_H
#define PLACE_BOMB_COMMAND_H

#include <roingine/commands/command.h>
#include <roingine/components/transform.h>

namespace roingine {
	class Transform;
}

namespace bomberman {
	class MovingEntity;
	struct PlayerInfo;

	class PlaceBombCommand final : public roingine::Command {
		roingine::GameObject m_Bomber;

	public:
		explicit PlaceBombCommand(roingine::GameObject bomber);

		void Execute() const override;
	};
}// namespace bomberman

#endif//PLACE_BOMB_COMMAND_H
