#ifndef UNASSIGNED_CONTROLLER_H
#define UNASSIGNED_CONTROLLER_H

#include <functional>
#include <optional>
#include <roingine/commands/registered_controller_command.h>

namespace bomberman {
	class Level;
}

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
		roingine::RegisteredControllerCommand m_AssignPlayer1;
		roingine::RegisteredControllerCommand m_AssignPlayer2;

		roingine::Controller *m_rpController;
	};
}// namespace bomberman

#endif//UNASSIGNED_CONTROLLER_H
