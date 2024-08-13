#ifndef ENEMY_H
#define ENEMY_H

#include <glm/vec2.hpp>
#include <roingine/components/animation_renderer.h>
#include <roingine/components/component.h>
#include <roingine/gameobject.h>
#include <utility>

namespace roingine {
	class AnimationRenderer;
	class Scene;
}// namespace roingine

namespace bomberman {
	class MovingEntity;
	class LevelFlyweight;
}// namespace bomberman

namespace bomberman {
	// not responsible for rendering, only for logic
	class Enemy final : public roingine::Component {
	public:
		enum class Speed { Slow, Normal, Fast };

		enum class Direction { Up, Down, Left, Right };

		static glm::vec2 DirectionToVec(Direction direction);

		struct EnemyInfo final {
			roingine::AnimationRenderer::AnimationInfo walkAnimInfo;
			roingine::AnimationRenderer::AnimationInfo deathAnimInfo;

			Speed speed;
			int   scoreOnKill;

			constexpr EnemyInfo(
			        roingine::AnimationRenderer::AnimationInfo walkAnimInfo,
			        roingine::AnimationRenderer::AnimationInfo deathAnimInfo, Speed speed, int scoreOnKill
			)
			    : walkAnimInfo{std::move(walkAnimInfo)}
			    , deathAnimInfo{std::move(deathAnimInfo)}
			    , speed{speed}
			    , scoreOnKill{scoreOnKill} {
			}
		};

		Enemy(roingine::GameObject gameObject, LevelFlyweight const &levelFlyweight, EnemyInfo const &info);

		// returns false if there was a collision and it couldn't move
		bool Move(glm::vec2 direction);

		void FlipDirection() const;

		static void SpawnEnemy(
		        roingine::Scene &scene, LevelFlyweight const &levelFlyweight, EnemyInfo const &info, glm::vec2 position
		);

		struct FreeDirections final {
			bool up{};
			bool down{};
			bool left{};
			bool right{};
		};

		[[nodiscard]]
		FreeDirections GetFreeDirection() const;

		[[nodiscard]]
		bool IsInOneTile() const;

	private:
		static float const c_Size;

		MovingEntity                              *m_rpMovingEntity;
		roingine::AnimationRenderer               *m_rpAnimRenderer;
		roingine::AnimationRenderer::AnimationInfo deathAnimInfo;
		int                                        m_ScoreOnKill{};
	};

	namespace enemy_type {
		Enemy::EnemyInfo const c_EnemyBalloom{
		        roingine::AnimationRenderer::AnimationInfo{
		                .fileName        = "res/img/enemy_balloom_walk.png",
		                .numFrames       = 3,
		                .secondsPerFrame = 0.1f
		        },
		        roingine::AnimationRenderer::AnimationInfo{
		                .fileName        = "res/img/enemy_death_salmon_color.png",
		                .numFrames       = 4,
		                .secondsPerFrame = 0.1f
		        },
		        Enemy::Speed::Slow, 100
		};
	}
}// namespace bomberman

#endif//ENEMY_H