#include "unassigned_controller.h"

#include <utility>

#include "level.h"
#include "player_info.h"

namespace bomberman {
	class AssignToPlayerCommand final : public roingine::Command {
		Level                                          *m_rpLevel;
		roingine::Controller                           *m_rpController;
		std::optional<UnassignedController::OnAssigned> m_OnAssigned;
		PlayerType                                      m_PlayerType;
		bool                                            m_IsPlayer1;

	public:
		AssignToPlayerCommand(
		        Level &m_r_level, roingine::Controller &controller, PlayerType playerType, bool isPlayer1,
		        std::optional<UnassignedController::OnAssigned> onAssigned = std::nullopt
		)
		    : m_rpLevel{&m_r_level}
		    , m_rpController{&controller}
		    , m_OnAssigned{std::move(onAssigned)}
		    , m_PlayerType{playerType}
		    , m_IsPlayer1{isPlayer1} {
		}

		void Execute() const override {
			if (m_IsPlayer1) {
				if (m_rpLevel->DoesPlayer1HaveController())
					return;

				m_rpLevel->SetPlayer1Controller(m_rpController);
			} else {
				if (m_rpLevel->DoesPlayer2HaveController())
					return;

				m_rpLevel->SetUpPlayer2(*m_rpController, m_PlayerType);
				PlayerInfoContainer::GetInstance().m_Player2Info = PlayerInfo{};
			}

			if (m_OnAssigned.has_value())
				m_OnAssigned.value()();
		}
	};

	UnassignedController::UnassignedController(
	        roingine::Controller *rpController, Level &level, std::optional<OnAssigned> const &onAssigned
	)
	    : m_AssignPlayer1Bomberman{rpController, roingine::ControllerButton::LeftShoulder, roingine::ButtonState::Pressed, std::make_unique<AssignToPlayerCommand>(level, *rpController, PlayerType::Bomberman, true, onAssigned)}
	    , m_AssignPlayer2Bomberman{rpController, roingine::ControllerButton::RightShoulder, roingine::ButtonState::Pressed, std::make_unique<AssignToPlayerCommand>(level, *rpController, PlayerType::Bomberman, false, onAssigned)}
	    , m_AssignPlayer2Balloom{rpController, roingine::ControllerButton::RightStick, roingine::ButtonState::Pressed, std::make_unique<AssignToPlayerCommand>(level, *rpController, PlayerType::Balloom, false, onAssigned)}
	    , m_rpController{rpController} {
	}

	roingine::Controller *UnassignedController::GetController() const {
		return m_rpController;
	}
}// namespace bomberman
