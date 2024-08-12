#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include "../event_queue.h"
#include "../finite_state_machine.h"
#include "../place_bomb_command.h"
#include "living_entity.h"

#include <roingine/components/component.h>
#include <roingine/controller.h>
#include <roingine/event_queue.h>

namespace roingine {
	class Transform;
	class AnimationRenderer;
	class RectCollider;
}// namespace roingine

namespace bomberman {
	class MovingEntity;

	class LevelFlyweight;

	class Bomberman;

	class BombermanFSMNode : public FiniteStateMachine<LivingEntityInstruction> {
		Bomberman const *m_rpBomberman;

	protected:
		[[nodiscard]]
		Bomberman const &GetBomberman() const;

	public:
		explicit BombermanFSMNode(Bomberman const &bomberman);
	};

	class BombermanIdle final : public BombermanFSMNode {
	public:
		using BombermanFSMNode::BombermanFSMNode;

		void OnEnter() override;

		void OnExit() override;

		[[nodiscard]]
		std::unique_ptr<FiniteStateMachine> Update(LivingEntityInstruction const &input) override;
	};

	class BombermanWalking final : public BombermanFSMNode {
	public:
		using BombermanFSMNode::BombermanFSMNode;

		[[nodiscard]]
		std::unique_ptr<FiniteStateMachine> Update(LivingEntityInstruction const &input) override;
	};

	class Bomberman final : public roingine::Component {
		roingine::EventHandlerHandle<event_queue::EventQueue> m_hExplosionHandler;
		roingine::RectCollider                               *m_rpRectCollider;
		MovingEntity                                         *m_rpMovingEntityComponent;
		roingine::AnimationRenderer                          *m_rpAnimRenderer;
		roingine::Transform                                  *m_rpTransform;
		std::unique_ptr<PlaceBombCommand>                     m_pPlaceBombCommand;
		glm::vec2                                             m_PreviousWalkSoundPosition{0.f, 0.f};

		static float const c_Size;
		static float const c_Speed;
		static float const c_WalkSoundDistance;

	public:
		Bomberman(roingine::GameObject gameObject, LevelFlyweight const &levelFlyweight);

		[[nodiscard]]
		roingine::AnimationRenderer &GetAnimRenderer() const;

		[[nodiscard]]
		roingine::Transform &GetTransform() const;

		void Move(glm::vec2 const &direction) const;

		void PlaceBomb() const;

		void Update() override;
	};
}// namespace bomberman

#endif//BOMBERMAN_H
