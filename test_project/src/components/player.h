#ifndef PLAYER_H
#define PLAYER_H

#include "../move_command.h"
#include "../place_bomb_command.h"
#include <glm/vec2.hpp>
#include <roingine/commands/registered_controller_command.h>
#include <roingine/commands/registered_keyboard_command.h>
#include <roingine/components/component.h>

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
		roingine::Controller             *m_rpController{};
		roingine::RectCollider const     *m_rpRectCollider;
		MovingEntity                     *m_rpMovingEntityComponent;
		std::unique_ptr<MoveCommand>      m_rpMoveUpCommand;
		std::unique_ptr<MoveCommand>      m_rpMoveDownCommand;
		std::unique_ptr<MoveCommand>      m_rpMoveLeftCommand;
		std::unique_ptr<MoveCommand>      m_rpMoveRightCommand;
		std::unique_ptr<PlaceBombCommand> m_pPlaceBombCommand;
		roingine::Transform              *m_rpTransform;
		roingine::Transform              *m_rpCameraTransform;
		roingine::AnimationRenderer      *m_rpAnimRenderer;
		glm::vec2                         m_PreviousWalkSoundPosition{0.f, 0.f};
		bool                              m_HasKeyboardSupport;

		static float const c_WalkSoundDistance;
		static float const c_Size;
		static float const c_WalkSpeed;

		[[nodiscard]]
		bool
		IsKbOrControllerInputDown(roingine::InputKeys key, roingine::ControllerButton button, bool checkIfHeld) const;

		void HandleInput() const;

	public:
		Player(roingine::GameObject &gameObject, roingine::Transform &cameraTransform,
		       LevelFlyweight const &levelFlyweight, bool keyboardSupported);

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
