#include "unassigned_controller.h"

#include <utility>

#include "level.h"

namespace bomberman {
	class AssignToPlayerCommand final : public roingine::Command {
		Level                                          *m_rpLevel;
		roingine::Controller                           *m_rpController;
		std::optional<UnassignedController::OnAssigned> m_OnAssigned;
		bool                                            m_IsPlayer1;

	public:
		AssignToPlayerCommand(
		        Level &m_r_level, roingine::Controller &controller, bool isPlayer1,
		        std::optional<UnassignedController::OnAssigned> onAssigned = std::nullopt
		)
		    : m_rpLevel{&m_r_level}
		    , m_rpController{&controller}
		    , m_OnAssigned{std::move(onAssigned)}
		    , m_IsPlayer1{isPlayer1} {
		}

		void Execute() override {
			if (m_IsPlayer1) {
				if (m_rpLevel->DoesPlayer1HaveController())
					return;

				m_rpLevel->SetPlayer1Controller(m_rpController);
			} else {
				if (m_rpLevel->DoesPlayer2HaveController())
					return;

				m_rpLevel->SetPlayer2Controller(m_rpController);
			}

			if (m_OnAssigned.has_value())
				m_OnAssigned.value()();
		}
	};

	UnassignedController::UnassignedController(
	        roingine::Controller *rpController, Level &level, std::optional<OnAssigned> const &onAssigned
	)
	    : m_AssignPlayer1{rpController, roingine::ControllerButton::LeftShoulder, roingine::ButtonState::Pressed, std::make_unique<AssignToPlayerCommand>(level, *rpController, true, onAssigned)}
	    , m_AssignPlayer2{rpController, roingine::ControllerButton::RightShoulder, roingine::ButtonState::Pressed, std::make_unique<AssignToPlayerCommand>(level, *rpController, false, onAssigned)}
	    , m_rpController{rpController} {
	}

	roingine::Controller *UnassignedController::GetController() const {
		return m_rpController;
	}
}// namespace bomberman
