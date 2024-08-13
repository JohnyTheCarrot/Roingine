#include "level_flyweight.h"

#include "../audio.h"
#include "../player_info.h"
#include "bomb.h"
#include "player.h"
#include "temporary_object.h"
#include "upgrade.h"

#include <algorithm>
#include <random>
#include <roingine/components/animation_renderer.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/texture_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>

// TODO: this file has a lot going on, there's probably a missing abstraction here. Maybe a class for the tiles?

namespace bomberman {
	constexpr int  c_DotWallSpacing{2};
	constexpr auto c_ExplosionAnimationFrames{7};
	constexpr auto c_ExplosionAnimationTime{0.05f};
	constexpr auto c_ExplosionTtl{c_ExplosionAnimationFrames * c_ExplosionAnimationTime};

	bool LevelFlyweight::DestroyTile(roingine::Scene &scene, int xIdx, int yIdx, bool isAnimated) {
		constexpr int   c_DestroyAnimFrames{5};
		constexpr float c_DestroyAnimTime{0.1f};
		constexpr float c_DestroyAnimTtl{c_DestroyAnimFrames * c_DestroyAnimTime};

		if (auto const inGrid{xIdx >= 0 && xIdx < c_LevelWidth && yIdx >= 0 && yIdx < c_LevelHeight}; !inGrid)
			return false;

		auto const arrIdx{static_cast<std::size_t>(xIdx + yIdx * c_LevelWidth)};
		auto      &tile{m_TileGrid.at(arrIdx)};
		if (tile == TileType::SolidWall)
			return false;

		if (tile == TileType::Nothing)
			return true;

		tile = TileType::Nothing;

		auto const gridPos{GridToPosition(xIdx, yIdx)};

		if (isAnimated) {
			auto goBrokenBricks{scene.AddGameObject()};
			goBrokenBricks.AddComponent<roingine::Transform>(gridPos, 0.f);
			goBrokenBricks.AddComponent<roingine::AnimationRenderer>(
			        roingine::AnimationRenderer::AnimationInfo{
			                .fileName        = "res/img/brick_wall_destroyed.png",
			                .numFrames       = c_DestroyAnimFrames,
			                .secondsPerFrame = c_DestroyAnimTime
			        },
			        c_TileSize, c_TileSize, roingine::ScalingMethod::NearestNeighbor
			);
			goBrokenBricks.AddComponent<TemporaryObject>(c_DestroyAnimTtl);
		}

		if (m_Upgrade.first == arrIdx) {
			auto upgradeObject{scene.AddGameObject()};
			upgradeObject.AddComponent<roingine::Transform>(gridPos, 0.f);
			upgradeObject.AddComponent<Upgrade>(m_Upgrade.second);
		} else if (m_DoorIndex == arrIdx) {
			auto door{scene.AddGameObject()};
			door.AddComponent<roingine::Transform>(gridPos, 0.f);
			door.AddComponent<roingine::TextureRenderer>(
			        "res/img/door.png", c_TileSize, c_TileSize, roingine::ScalingMethod::NearestNeighbor
			);
			auto &collider{door.AddComponent<roingine::RectCollider>(c_TileSize, c_TileSize)};
			collider.SetCallback([](roingine::GameObject other, glm::vec2, roingine::RectCollider::HitDirection) {
				if (other.GetOptionalComponent<Player>() == nullptr)
					return;

				event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::DoorUsed>();
			});
		}

		return true;
	}

	bool LevelFlyweight::IsPointInWall(glm::vec2 const &point) const {
		auto const ownWorldPos{m_rpTransform->GetWorldPosition()};
		auto const relativePos{point - ownWorldPos};
		auto const xIndex{static_cast<int>(relativePos.x / c_TileSize)};
		auto const yIndex{static_cast<int>(relativePos.y / c_TileSize)};

		if (auto const inGrid{xIndex >= 0 && xIndex < c_LevelWidth && yIndex >= 0 && yIndex < c_LevelHeight}; !inGrid)
			return false;

		return m_TileGrid.at(xIndex + yIndex * c_LevelWidth) != TileType::Nothing;
	}

	void LevelFlyweight::BombDetonatedHandler(event_queue::BombDetonatedData const &data) {
		auto &bombs{data.rpBomber->m_IsPlayer1 ? m_BombsPlayer1 : m_BombsPlayer2};

		if (auto const it{std::ranges::find_if(bombs, [&data](auto const &pair) { return pair.first == data.bombId; })};
		    it != bombs.end()) {
			bombs.erase(it);
		}

		auto const ownWorldPos{m_rpTransform->GetWorldPosition()};
		auto const relativePos{data.position - ownWorldPos};
		auto const xIndex{static_cast<int>(relativePos.x / c_TileSize)};
		auto const yIndex{static_cast<int>(std::floor(relativePos.y / c_TileSize))};
		int const  bombRange{data.rpBomber->m_BombRange};

		audio::AudioServiceLocator::GetService().Play(audio::Sound::BombExplode);

		{
			auto *const activeScene{roingine::SceneManager::GetInstance().GetActive()};
			auto        centerExplosion{activeScene->AddGameObject()};
			centerExplosion.AddComponent<roingine::Transform>(SnapToGrid(relativePos), 0.f);
			centerExplosion.AddComponent<roingine::AnimationRenderer>(
			        roingine::AnimationRenderer::AnimationInfo{
			                .fileName        = "res/img/explosion_center.png",
			                .numFrames       = c_ExplosionAnimationFrames,
			                .secondsPerFrame = c_ExplosionAnimationTime
			        },
			        c_TileSize, c_TileSize, roingine::ScalingMethod::NearestNeighbor
			);
			centerExplosion.AddComponent<TemporaryObject>(c_ExplosionTtl);
		}

		auto const relativePosGrid{GridToPosition(xIndex, yIndex)};
		auto       explosionLeft{relativePosGrid.x};
		auto       explosionRight{relativePosGrid.x + c_TileSize};
		auto       explosionTop{relativePosGrid.y};
		auto       explosionBottom{relativePosGrid.y + c_TileSize};

		// destroy tiles to the left
		auto const nExplodedLeft{ExplodeTiles(relativePos, true, -bombRange, std::greater_equal<int>{}, xIndex, yIndex)
		};
		explosionLeft -= static_cast<float>(nExplodedLeft) * c_TileSize;

		// destroy tiles to the right
		auto const nExplodedRight{ExplodeTiles(relativePos, true, bombRange, std::less_equal<int>{}, xIndex, yIndex)};
		explosionRight += static_cast<float>(nExplodedRight) * c_TileSize;

		// destroy tiles above
		auto const nExplodedTop{ExplodeTiles(relativePos, false, -bombRange, std::greater_equal<int>{}, xIndex, yIndex)
		};
		explosionTop -= static_cast<float>(nExplodedTop) * c_TileSize;

		// destroy tiles below
		auto const nExplodedBottom{ExplodeTiles(relativePos, false, bombRange, std::less_equal<int>{}, xIndex, yIndex)};
		explosionBottom += static_cast<float>(nExplodedBottom) * c_TileSize;

		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::Explosion>(
		        glm::vec2{explosionLeft, relativePosGrid.y}, glm::vec2{explosionRight, relativePosGrid.y + c_TileSize},
		        glm::vec2{relativePosGrid.x, explosionTop}, glm::vec2{relativePosGrid.x + c_TileSize, explosionBottom}
		);
	}

	void LevelFlyweight::BombPlaceRequestHandler(event_queue::BombPlaceRequestData const &data) {
		auto const activeScene{roingine::SceneManager::GetInstance().GetActive()};

		auto const *rpPlayerInfo{data.rpBomber};
		if (m_BombsPlayer1.size() + 1 > static_cast<size_t>(rpPlayerInfo->m_MaxBombs))
			return;

		auto const pos{SnapToGrid(data.position)};
		auto       bomb{activeScene->AddGameObject()};
		auto      &bombComp{bomb.AddComponent<Bomb>(pos, *data.rpBomber)};

		if (rpPlayerInfo->m_IsPlayer1) {
			m_BombsPlayer1.emplace_back(bombComp.GetBombId(), &bombComp);
		} else {
			m_BombsPlayer2.emplace_back(bombComp.GetBombId(), &bombComp);
		}
	}

	int LevelFlyweight::ExplodeTiles(
	        glm::vec2 startPos, bool isX, int range, std::function<bool(int, int)> const &comp, int xIndex, int yIndex
	) {
		auto *const activeScene{roingine::SceneManager::GetInstance().GetActive()};

		auto const spawnExplosion{[&](bool isEnd, int offsetFactor) {
			auto       middleExplosion{activeScene->AddGameObject()};
			auto const angle{[isX, startPos, range]() {
				if (isX)
					return range < 0 ? 0.f : 180.f;

				return range < 0 ? 90.f : 270.f;
			}()};

			auto &transform{middleExplosion.AddComponent<roingine::Transform>(
			        SnapToGrid(startPos) +
			                glm::vec2{
			                        c_TileSize * static_cast<float>(offsetFactor * isX),
			                        c_TileSize * static_cast<float>(offsetFactor * !isX)
			                },
			        angle
			)};
			transform.SetPivot(c_TileSize / 2.f, c_TileSize / 2.f);
			middleExplosion.AddComponent<roingine::AnimationRenderer>(
			        roingine::AnimationRenderer::AnimationInfo{
			                .fileName        = isEnd ? "res/img/explosion_end.png" : "res/img/explosion_middle.png",
			                .numFrames       = c_ExplosionAnimationFrames,
			                .secondsPerFrame = c_ExplosionAnimationTime
			        },
			        c_TileSize, c_TileSize, roingine::ScalingMethod::NearestNeighbor
			);
			middleExplosion.AddComponent<TemporaryObject>(c_ExplosionTtl);
		}};

		auto const idxIncrement{range > 0 ? 1 : -1};
		auto const startIdx{isX ? xIndex : yIndex};
		auto const endIdx{startIdx + range};

		for (int idx{startIdx + idxIncrement}; comp(idx, endIdx); idx += idxIncrement) {
			auto const indexIfX{yIndex * c_LevelWidth + idx};
			auto const indexIfY{idx * c_LevelWidth + xIndex};
			auto const tileIndex{isX ? indexIfX : indexIfY};

			if (auto const tile{m_TileGrid.at(tileIndex)}; tile != TileType::Nothing)
				break;

			spawnExplosion(idx == endIdx, idx - startIdx);
		}

		bool hasSeenWall{false};
		int  explodedTiles{0};
		for (int idx{startIdx + idxIncrement}; comp(idx, startIdx + range); idx += idxIncrement) {
			auto const tileXIndex{isX ? idx : xIndex};
			auto const tileYIndex{isX ? yIndex : idx};

			if (!DestroyTile(*activeScene, tileXIndex, tileYIndex))
				break;

			if (auto const tile{m_TileGrid.at(tileXIndex + tileYIndex * c_LevelWidth)};
			    tile != TileType::Nothing || hasSeenWall) {
				hasSeenWall = true;
				continue;
			}

			++explodedTiles;
		}

		return explodedTiles;
	}

	LevelFlyweight::LevelFlyweight(roingine::GameObject gameObject, UpgradeType upgrade)
	    : Component{gameObject}
	    , m_hBombPlaceRequestHandler{event_queue::EventQueue::GetInstance()
	                                         .AttachEventHandler<event_queue::EventType::BombPlaceRequest>(
	                                                 [this](auto const &data) { BombPlaceRequestHandler(data); }
	                                         )}
	    , m_hBombDetonatedHandler{event_queue::EventQueue::GetInstance()
	                                      .AttachEventHandler<event_queue::EventType::BombDetonated>(
	                                              [this](auto const &data) { BombDetonatedHandler(data); }
	                                      )}
	    , m_hLastBombDetonationHandler{event_queue::EventQueue::GetInstance()
	                                           .AttachEventHandler<event_queue::EventType::DetonateLastBombRequest>(
	                                                   [this](auto const &data) {
		                                                   auto &bombs{
		                                                           data.rpBomber->m_IsPlayer1 ? m_BombsPlayer1
		                                                                                      : m_BombsPlayer2
		                                                   };

		                                                   if (bombs.empty())
			                                                   return;

		                                                   bombs.back().second->Explode();
	                                                   }
	                                           )}
	    , m_SolidWallTexture{"res/img/wall.png", roingine::ScalingMethod::NearestNeighbor}
	    , m_BrickWallTexture{"res/img/brick_wall.png", roingine::ScalingMethod::NearestNeighbor}
	    , m_rpTransform{&gameObject.GetComponent<roingine::Transform>()} {
		m_TileGrid.resize(c_LevelWidth * c_LevelHeight);

		// Generate random brick walls
		std::ranges::generate(m_TileGrid, [this] {
			static std::random_device          rd;
			static std::mt19937                gen(rd());
			static std::bernoulli_distribution d(0.30);

			return d(gen) ? TileType::BrickWall : TileType::Nothing;
		});

		// Surround the level with solid walls
		std::fill_n(m_TileGrid.begin(), c_LevelWidth, TileType::SolidWall);
		std::fill_n(m_TileGrid.end() - c_LevelWidth, c_LevelWidth, TileType::SolidWall);
		for (int y{1}; y < c_LevelHeight - 1; ++y) {
			m_TileGrid.at(y * c_LevelWidth)                    = TileType::SolidWall;
			m_TileGrid.at(y * c_LevelWidth + c_LevelWidth - 1) = TileType::SolidWall;
		}

		// Dot walls around the level
		for (int x{c_DotWallSpacing}; x < c_LevelWidth - 1; x += c_DotWallSpacing) {
			for (int y{c_DotWallSpacing}; y < c_LevelHeight - 1; y += c_DotWallSpacing) {
				m_TileGrid.at(y * c_LevelWidth + x) = TileType::SolidWall;
			}
		}

		static std::mt19937                        generator(std::random_device{}());
		std::uniform_int_distribution<std::size_t> distribution(0, m_TileGrid.size() - 1);

		while (true) {
			auto const idx{distribution(generator)};
			if (m_TileGrid.at(idx) != TileType::BrickWall)
				continue;

			m_Upgrade = {idx, upgrade};
			break;
		}

		while (true) {
			auto const idx{distribution(generator)};
			if (m_TileGrid.at(idx) != TileType::BrickWall)
				continue;

			m_DoorIndex = idx;
			break;
		}

		// for testing upgrade spawning:

		// auto *scene{gameObject.GetScene()};
		// for (int y{1}; y < c_LevelHeight - 1; ++y) {
		// 	for (int x{1}; x < c_LevelWidth - 1; ++x) {
		// 		if (m_TileGrid.at(y * c_LevelWidth + x) == TileType::BrickWall)
		// 			DestroyTile(*scene, x, y);
		// 	}
		// }
	}

	void LevelFlyweight::Render() const {
		roingine::TransformContext context{*m_rpTransform};

		for (int y{0}; y < c_LevelHeight; ++y) {
			for (int x{0}; x < c_LevelWidth; ++x) {
				auto const     &tile{m_TileGrid.at(y * c_LevelWidth + x)};
				glm::vec2 const pos{static_cast<float>(x) * c_TileSize, static_cast<float>(y) * c_TileSize};

				switch (tile) {
					case TileType::SolidWall:
						m_SolidWallTexture.Render(pos, c_TileSize, c_TileSize);
						break;
					case TileType::BrickWall:
						m_BrickWallTexture.Render(pos, c_TileSize, c_TileSize);
						break;
					case TileType::Nothing:
						break;
				}
			}
		}
	}

	bool LevelFlyweight::IsCollidingWith(roingine::RectCollider const &collider) const {
		auto const      worldPos{collider.GetTransform().GetWorldPosition()};
		glm::vec2 const topLeft{worldPos.x, worldPos.y};
		glm::vec2 const topRight{worldPos.x + collider.GetWidth(), worldPos.y};
		glm::vec2 const bottomLeft{worldPos.x, worldPos.y + collider.GetHeight()};
		glm::vec2 const bottomRight{worldPos.x + collider.GetWidth(), worldPos.y + collider.GetHeight()};

		return IsPointInWall(topLeft) || IsPointInWall(topRight) || IsPointInWall(bottomLeft) ||
		       IsPointInWall(bottomRight);
	}

	std::optional<glm::vec2> LevelFlyweight::GetCollisionPoint(glm::vec2 origin, float width, float height) const {
		glm::vec2 const topLeft{origin.x, origin.y};
		glm::vec2 const topRight{origin.x + width, origin.y};
		glm::vec2 const bottomLeft{origin.x, origin.y + height};
		glm::vec2 const bottomRight{origin.x + width, origin.y + height};

		if (!IsPointInWall(topLeft) && !IsPointInWall(topRight) && !IsPointInWall(bottomLeft) &&
		    !IsPointInWall(bottomRight))
			return std::nullopt;

		auto const ownWorldPos{m_rpTransform->GetWorldPosition()};

		// get side of the player that collided with the wall
		auto const top{origin.y};
		auto const bottom{origin.y + height};
		auto const left{origin.x};
		auto const right{origin.x + width};

		auto const rightOfTile{std::ceil(left / c_TileSize) * c_TileSize};
		auto const leftOfTile{std::floor(right / c_TileSize) * c_TileSize};
		auto const bottomOfTile{std::ceil(top / c_TileSize) * c_TileSize};
		auto const topOfTile = std::floor(bottom / c_TileSize) * c_TileSize;

		glm::vec2  collisionPoint{origin.x, origin.y};
		auto const distanceTop{std::abs(top - (ownWorldPos.y + bottomOfTile))};
		auto const distanceBottom{std::abs(bottom - (ownWorldPos.y + topOfTile))};
		auto const distanceLeft{std::abs(left - (ownWorldPos.x + rightOfTile))};
		auto const distanceRight{std::abs(right - (ownWorldPos.x + leftOfTile))};

		constexpr float c_CollisionOffset{1.0f};

		if (std::min(distanceTop, distanceBottom) < std::min(distanceLeft, distanceRight)) {
			if (distanceTop < distanceBottom) {
				collisionPoint.y = ownWorldPos.y + bottomOfTile + c_CollisionOffset;
				return collisionPoint;
			}

			collisionPoint.y = ownWorldPos.y + topOfTile - height - c_CollisionOffset;
			return collisionPoint;
		}

		if (distanceLeft < distanceRight) {
			collisionPoint.x = ownWorldPos.x + rightOfTile + c_CollisionOffset;
			return collisionPoint;
		}

		collisionPoint.x = ownWorldPos.x + leftOfTile - width - c_CollisionOffset;
		return collisionPoint;
	}

	LevelFlyweight::TileType LevelFlyweight::GetTileType(int gridX, int gridY) const {
		if (gridX < 0 || gridX >= c_LevelWidth || gridY < 0 || gridY >= c_LevelHeight)
			return TileType::SolidWall;

		return m_TileGrid.at(gridX + gridY * c_LevelWidth);
	}

	glm::vec2 LevelFlyweight::GridToPosition(int x, int y) const {
		return m_rpTransform->GetWorldPosition() +
		       glm::vec2{static_cast<float>(x) * c_TileSize, static_cast<float>(y) * c_TileSize};
	}

	glm::vec2 LevelFlyweight::SnapToGrid(glm::vec2 position) const {
		auto const relativePos{position - m_rpTransform->GetWorldPosition()};
		auto const x{std::floor(relativePos.x / c_TileSize) * c_TileSize};
		auto const y{std::floor(relativePos.y / c_TileSize) * c_TileSize};

		return {x, y};
	}

	std::pair<int, int> LevelFlyweight::PositionToGrid(glm::vec2 position) const {
		auto const relativePos{position - m_rpTransform->GetWorldPosition()};
		auto const x{static_cast<int>(std::floor(relativePos.x / c_TileSize))};
		auto const y{static_cast<int>(std::floor(relativePos.y / c_TileSize))};

		return {x, y};
	}

	glm::vec2 LevelFlyweight::GetRandomEmptyTilePos() const {
		static std::random_device            rd;
		static std::mt19937                  gen(rd());
		static std::uniform_int_distribution xDist{0, c_LevelWidth - 1};
		static std::uniform_int_distribution yDist{0, c_LevelHeight - 1};

		auto const isNotEmpty{[this](int x, int y) { return GetTileType(x, y) != TileType::Nothing; }};

		auto x{xDist(gen)};
		auto y{yDist(gen)};

		while (isNotEmpty(x, y)) {
			x = xDist(gen);
			y = yDist(gen);
		}

		return GridToPosition(x, y);
	}
}// namespace bomberman
