#include "enemy.h"

#include "enemy_ai.h"
#include "level_flyweight.h"
#include "moving_entity.h"
#include "roingine/scene.h"

#include <glm/common.hpp>
#include <roingine/components/animation_renderer.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>

namespace bomberman {
	glm::vec2 Enemy::DirectionToVec(Direction direction) {
		switch (direction) {
			case Direction::Up:
				return {0, 1};
			case Direction::Down:
				return {0, -1};
			case Direction::Left:
				return {-1, 0};
			default:
				return {1, 0};
		}
	}

	Enemy::Enemy(roingine::GameObject gameObject, LevelFlyweight const &levelFlyweight, EnemyInfo const &info)
	    : Component{gameObject}
	    , m_rpMovingEntity{&gameObject.AddComponent<MovingEntity>(
	              levelFlyweight,
	              [&info] {
		              switch (info.speed) {
			              default:
				              return 150.f;
			              case Speed::Normal:
				              return 200.f;
			              case Speed::Fast:
				              return 250.f;
		              }
	              }()
	      )}
	    , m_rpAnimRenderer{&gameObject.AddComponent<roingine::AnimationRenderer>(
	              info.walkAnimInfo, c_Size, c_Size, roingine::ScalingMethod::NearestNeighbor
	      )}
	    , deathAnimInfo{info.deathAnimInfo}
	    , m_ScoreOnKill{info.scoreOnKill} {
	}

	float const Enemy::c_Size{70.f};

	bool Enemy::Move(glm::vec2 direction) {
		return m_rpMovingEntity->Move(direction);
	}

	void Enemy::FlipDirection() const {
		m_rpAnimRenderer->SetFlipped(!m_rpAnimRenderer->GetFlipped());
	}

	void Enemy::SpawnEnemy(
	        roingine::Scene &scene, LevelFlyweight const &levelFlyweight, EnemyInfo const &info, glm::vec2 position
	) {
		auto enemyObject{scene.AddGameObject()};
		enemyObject.AddComponent<roingine::Transform>(position, 0.f);
		enemyObject.AddComponent<roingine::RectCollider>(c_Size, c_Size);
		enemyObject.AddComponent<Enemy>(levelFlyweight, info);
		enemyObject.AddComponent<EnemyAi>(EnemyAi::AIType::Simple);
	}

	Enemy::FreeDirections Enemy::GetFreeDirection() const {
		auto const &levelFlyweight{m_rpMovingEntity->GetLevelFlyweight()};
		auto const &transform{GetGameObject().GetComponent<roingine::Transform>()};
		auto const  gridPos{levelFlyweight.PositionToGrid(transform.GetWorldPosition())};

		auto const isFree{[&](int offsetX, int offsetY) {
			return levelFlyweight.GetTileType(
			               static_cast<int>(gridPos.first) + offsetX, static_cast<int>(gridPos.second) + offsetY
			       ) == LevelFlyweight::TileType::Nothing;
		}};

		return {.up = isFree(0, 1), .down = isFree(0, -1), .left = isFree(-1, 0), .right = isFree(1, 0)};
	}

	bool Enemy::IsInOneTile() const {
		auto const &levelFlyweight{m_rpMovingEntity->GetLevelFlyweight()};
		auto const &transform{GetGameObject().GetComponent<roingine::Transform>()};
		auto const  pos{transform.GetWorldPosition()};
		auto const  topLeft{pos};
		auto const  topRight{pos + glm::vec2{c_Size, 0}};
		auto const  bottomLeft{pos + glm::vec2{0, c_Size}};
		auto const  bottomRight{pos + glm::vec2{c_Size, c_Size}};

		auto const topLeftGrid{levelFlyweight.PositionToGrid(topLeft)};
		auto const topRightGrid{levelFlyweight.PositionToGrid(topRight)};
		auto const bottomLeftGrid{levelFlyweight.PositionToGrid(bottomLeft)};
		auto const bottomRightGrid{levelFlyweight.PositionToGrid(bottomRight)};

		return topLeftGrid == topRightGrid && topLeftGrid == bottomLeftGrid && topLeftGrid == bottomRightGrid;
	}
}// namespace bomberman
