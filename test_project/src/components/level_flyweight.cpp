#include "level_flyweight.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>

namespace bomberman {
	constexpr int c_PlayerBreathingRoomTiles{3};
	constexpr int c_DotWallSpacing{2};

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
		// get bomb position in the level grid
		auto const    ownWorldPos{m_rpTransform->GetWorldPosition()};
		auto const    relativePos{data.position - ownWorldPos};
		auto const    xIndex{static_cast<int>(relativePos.x / c_TileSize)};
		auto const    yIndex{static_cast<int>(std::floor(relativePos.y / c_TileSize))};
		constexpr int c_BombRange{1};

		// returns false if the tile is a solid wall, which means the explosion stops there
		auto const destroyTile{[this](int x, int y) -> bool {
			if (auto const inGrid{x >= 0 && x < c_LevelWidth && y >= 0 && y < c_LevelHeight}; !inGrid)
				return false;

			auto &tile{m_TileGrid.at(x + y * c_LevelWidth)};
			if (tile == TileType::SolidWall)
				return false;

			tile = TileType::Nothing;
			return true;
		}};

		// destroy tiles to the left
		for (int x{xIndex - 1}; x >= std::max(0, xIndex - c_BombRange); --x) {
			if (!destroyTile(x, yIndex))
				break;
		}

		// destroy tiles to the right
		for (int x{xIndex + 1}; x <= std::min(c_LevelWidth - 1, xIndex + c_BombRange); ++x) {
			if (!destroyTile(x, yIndex))
				break;
		}

		// destroy tiles above
		for (int y{yIndex - 1}; y >= std::max(0, yIndex - c_BombRange); --y) {
			if (!destroyTile(xIndex, y))
				break;
		}

		// destroy tiles below
		for (int y{yIndex + 1}; y <= std::min(c_LevelHeight - 1, yIndex + c_BombRange); ++y) {
			if (!destroyTile(xIndex, y))
				break;
		}
	}

	LevelFlyweight::LevelFlyweight(roingine::GameObject &gameObject)
	    : Component{gameObject}
	    , m_hBombDetonatedHandler{event_queue::EventQueue::GetInstance()
	                                      .AttachEventHandler<event_queue::EventType::BombDetonated>(
	                                              [this](auto const &data) { BombDetonatedHandler(data); }
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

		// Ensure the player isn't in a wall or too close to one
		std::fill_n(m_TileGrid.end() - 2 * c_LevelWidth + 1, c_PlayerBreathingRoomTiles, TileType::Nothing);

		// Dot walls around the level
		for (int x{c_DotWallSpacing}; x < c_LevelWidth - 1; x += c_DotWallSpacing) {
			for (int y{c_DotWallSpacing}; y < c_LevelHeight - 1; y += c_DotWallSpacing) {
				m_TileGrid.at(y * c_LevelWidth + x) = TileType::SolidWall;
			}
		}
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
}// namespace bomberman
