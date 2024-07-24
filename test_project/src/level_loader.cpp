#include "level_loader.h"

#include <random>
#include <roingine/components/texture_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace bomberman::level_loader {
	constexpr int    c_LevelWidth{31};
	constexpr int    c_LevelHeight{13};
	constexpr int    c_WallGridInterval{2};
	constexpr float  c_TileSize{50.f};
	constexpr double c_BrickWallChance{.25};

	void CreateWall(roingine::Scene &scene, int tileX, int tileY) {
		auto const x{static_cast<float>(tileX) * c_TileSize};
		auto const y{static_cast<float>(tileY) * c_TileSize};

		auto wall{scene.AddGameObject()};
		wall.AddComponent<roingine::Transform>(glm::vec2{x, y}, 0.f);
		wall.AddComponent<roingine::TextureRenderer>(
		        "res/img/wall.png", c_TileSize, c_TileSize, roingine::ScalingMethod::NearestNeighbor
		);
	}

	void CreateBrickWall(roingine::Scene &scene, int tileX, int tileY) {
		auto const x{static_cast<float>(tileX) * c_TileSize};
		auto const y{static_cast<float>(tileY) * c_TileSize};

		auto wall{scene.AddGameObject()};
		wall.AddComponent<roingine::Transform>(glm::vec2{x, y}, 0.f);
		wall.AddComponent<roingine::TextureRenderer>(
		        "res/img/brick_wall.png", c_TileSize, c_TileSize, roingine::ScalingMethod::NearestNeighbor
		);
	}

	void LoadScene(roingine::Scene &scene, LevelLoadInfo const &) {
		for (int x{0}; x < c_LevelWidth; ++x) {
			CreateWall(scene, x, 0);
			CreateWall(scene, x, c_LevelHeight - 1);
		}

		for (int y{0}; y < c_LevelHeight; ++y) {
			CreateWall(scene, 0, y);
			CreateWall(scene, c_LevelWidth - 1, y);
		}

		for (int x{c_WallGridInterval}; x < c_LevelWidth - 1; x += c_WallGridInterval) {
			for (int y{c_WallGridInterval}; y < c_LevelHeight - 1; y += c_WallGridInterval) { CreateWall(scene, x, y); }
		}

		static std::random_device          rd;
		static std::mt19937                gen(rd());
		static std::bernoulli_distribution distribution{c_BrickWallChance};

		for (int x{1}; x < c_LevelWidth - 1; ++x) {
			for (int y{1}; y < c_LevelHeight - 1; ++y) {
				if ((x % c_WallGridInterval != 0 || y % c_WallGridInterval != 0) && distribution(gen))
					CreateBrickWall(scene, x, y);
			}
		}
	}
}// namespace bomberman::level_loader
