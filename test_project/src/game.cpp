#include "game.h"

#include "components/player.h"
#include "level_loader.h"

#include <roingine/scene.h>
#include <roingine/scene_manager.h>

namespace bomberman {
	Game::Game(roingine::Scene &&scene, int windowWidth, int windowHeight) {
		LevelLoadInfo const levelLoadInfo{windowWidth, windowHeight};

		Level const level{scene};
		level.Load(levelLoadInfo);

		roingine::SceneManager::GetInstance().SetActive(std::move(scene));
	}
}// namespace bomberman
