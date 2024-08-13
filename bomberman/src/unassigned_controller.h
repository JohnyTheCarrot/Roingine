#ifndef UNASSIGNED_CONTROLLER_H
#define UNASSIGNED_CONTROLLER_H

#include "level.h"

#include <functional>
#include <optional>
#include <roingine/commands/registered_controller_command.h>

namespace roingine {
	class Controller;
}

namespace bomberman {
	class UnassignedController final {
	public:
		using OnAssigned = std::function<void()>;

		UnassignedController(
		        roingine::Controller *rpController, Level &level,
		        std::optional<OnAssigned> const &onAssigned = std::nullopt
		);

		[[nodiscard]]
		roingine::Controller *GetController() const;

	private:
		roingine::RegisteredControllerCommand m_AssignPlayer1Bomberman;

		roingine::RegisteredControllerCommand m_AssignPlayer2Bomberman;
		roingine::RegisteredControllerCommand m_AssignPlayer2Balloom;

		roingine::Controller *m_rpController;
	};
}// namespace bomberman

#endif//UNASSIGNED_CONTROLLER_H
