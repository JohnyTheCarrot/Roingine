#include "level_flyweight.h"

#include <algorithm>
#include <random>
#include <roingine/components/transform.h>

namespace bomberman {
	constexpr int c_PlayerBreathingRoomTiles{3};
	constexpr int c_DotWallSpacing{2};

	LevelFlyweight::LevelFlyweight(roingine::GameObject &gameObject)
	    : Component{gameObject}
	    , m_rpTransform{&gameObject.GetComponent<roingine::Transform>()}
	    , m_SolidWallTexture{"res/img/wall.png", roingine::ScalingMethod::NearestNeighbor}
	    , m_BrickWallTexture{"res/img/brick_wall.png", roingine::ScalingMethod::NearestNeighbor} {
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
}// namespace bomberman
