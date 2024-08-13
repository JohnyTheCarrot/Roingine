#ifndef PLAYER_H
#define PLAYER_H

#include "../event_queue.h"
#include "../place_bomb_command.h"

#include <roingine/commands/command.h>
#include <roingine/commands/registered_controller_command.h>
#include <roingine/commands/registered_keyboard_command.h>
#include <roingine/components/component.h>
#include <roingine/gameobject.h>

namespace bomberman {
	class LivingEntity;
}

namespace bomberman {
	class Bomberman;
}

namespace bomberman {
	class LevelFlyweight;
}// namespace bomberman

namespace roingine {
	class RectCollider;
	class AnimationRenderer;
	class Transform;
}// namespace roingine

namespace bomberman {
	class MovingEntity;

	class Player final : public roingine::Component {
		LivingEntity                      *m_rpLivingEntityComponent;
		std::unique_ptr<roingine::Command> m_pMoveUpCommand;
		std::unique_ptr<roingine::Command> m_pMoveDownCommand;
		std::unique_ptr<roingine::Command> m_pMoveLeftCommand;
		std::unique_ptr<roingine::Command> m_pMoveRightCommand;
		std::unique_ptr<roingine::Command> m_pIdleCommand;
		std::unique_ptr<roingine::Command> m_pPrimaryActionCommand;
		std::unique_ptr<roingine::Command> m_pSeconaryActionCommand;

		roingine::Controller                                 *m_rpController{};
		roingine::EventHandlerHandle<event_queue::EventQueue> m_hExplosionHandler;
		roingine::Transform                                  *m_rpCameraTransform;
		roingine::Transform                                  *m_rpTransform;
		roingine::AnimationRenderer                          *m_rpAnimRenderer;
		std::optional<float>                                  m_HurtingTimeLeft{std::nullopt};
		float                                                 m_FlashTime{0.0f};
		bool                                                  m_IsPlayerOne{false};

		static float const c_WalkSoundDistance;
		static float const c_Size;
		static float const c_WalkSpeed;
		static float const c_HurtingTimeSec;
		static float const c_FlashDurationSec;

		[[nodiscard]]
		bool
		IsKbOrControllerInputDown(roingine::InputKeys key, roingine::ControllerButton button, bool checkIfHeld) const;

		void HandleInput() const;

	public:
		Player(roingine::GameObject gameObject, roingine::Transform &cameraTransform,
		       LevelFlyweight const &levelFlyweight, bool isPlayerOne);

		void TieToController(roingine::Controller *rpController);

		void DisconnectController();

		[[nodiscard]]
		bool HasSpecificController(roingine::Controller const *rpController) const;

		[[nodiscard]]
		bool HasConnectedController() const;

		[[nodiscard]]
		bool IsPlayerOne() const;

		void Hurt();

		void Update() override;

		[[nodiscard]]
		PlayerInfo &GetPlayerInfo() const;
	};
}// namespace bomberman

#endif//PLAYER_H
