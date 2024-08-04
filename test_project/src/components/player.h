#ifndef PLAYER_H
#define PLAYER_H

#include <roingine/commands/registered_controller_command.h>
#include <roingine/commands/registered_keyboard_command.h>
#include <roingine/components/component.h>

namespace roingine {
	class RectCollider;
}

namespace bomberman {
	class LevelFlyweight;
}

namespace roingine {
	class Transform;
}

namespace bomberman {
	class MovingEntity;

	class PlayerKeyboardCommands final {
		roingine::RegisteredKeyboardCommand m_KeyboardUpCommand;
		roingine::RegisteredKeyboardCommand m_KeyboardDownCommand;
		roingine::RegisteredKeyboardCommand m_KeyboardLeftCommand;
		roingine::RegisteredKeyboardCommand m_KeyboardRightCommand;

	public:
		explicit PlayerKeyboardCommands(MovingEntity *rpMovingEntityComponent);
	};

	struct ControllerCommands final {
		roingine::RegisteredControllerCommand m_ControllerUpCommand;
		roingine::RegisteredControllerCommand m_ControllerDownCommand;
		roingine::RegisteredControllerCommand m_ControllerLeftCommand;
		roingine::RegisteredControllerCommand m_ControllerRightCommand;
		roingine::Controller                 *m_rpController;

		explicit ControllerCommands(roingine::Controller *rpController, MovingEntity *rpMovingEntityComponent);
	};

	class Player final : public roingine::Component {
		std::optional<PlayerKeyboardCommands> m_KeyboardCommands;
		std::optional<ControllerCommands>     m_ControllerCommands;
		roingine::Transform                  *m_rpTransform;
		roingine::Transform                  *m_rpCameraTransform;
		MovingEntity                         *m_rpMovingEntityComponent;
		roingine::RectCollider const         *m_rpRectCollider;

	public:
		Player(roingine::GameObject &gameObject, roingine::Transform &cameraTransform,
		       MovingEntity *rpMovingEntityComponent, bool isPlayer1);

		void TieToController(roingine::Controller *rpController);

		void DisconnectController();

		[[nodiscard]]
		bool HasSpecificController(roingine::Controller const *rpController) const;

		[[nodiscard]]
		bool HasConnectedController() const;

		void Update() override;
	};
}// namespace bomberman

#endif//PLAYER_H
