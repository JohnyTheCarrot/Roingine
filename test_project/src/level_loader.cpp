#include "level_loader.h"

#include "components/level_flyweight.h"
#include "components/moving_entity.h"
#include "components/player.h"

#include <roingine/components/camera.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace bomberman {
	constexpr int c_PlayerStartX{1};
	constexpr int c_PlayerStartY{LevelFlyweight::c_LevelHeight - 2};

	void Level::SpawnPlayer(bool hasKeyboardSupport, int viewX, int viewY, int viewWidth, int viewHeight) const {
		auto  cameraObject{m_rpScene->AddGameObject()};
		auto &cameraTransform{cameraObject.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, 0.f)};

		&cameraObject.AddComponent<roingine::Camera>(viewX, viewY, viewWidth, viewHeight);

		auto player{m_rpScene->AddGameObject()};
		player.AddComponent<roingine::Transform>(
		        glm::vec2{c_PlayerStartX * LevelFlyweight::c_TileSize, c_PlayerStartY * LevelFlyweight::c_TileSize}, 0.f
		);
		player.AddComponent<roingine::RectRenderer>(50.f, 50.f);
		auto *rpMovingEntity{&player.AddComponent<MovingEntity>(200.f)};
		player.AddComponent<Player>(cameraTransform, rpMovingEntity, hasKeyboardSupport);
	}

	Level::Level(roingine::Scene &scene)
	    : m_rpScene{&scene} {
	}

	void Level::Load(LevelLoadInfo const &loadInfo) const {
		SpawnPlayer(true, 0, 0, loadInfo.windowWidth, loadInfo.windowHeight);

		auto level{m_rpScene->AddGameObject()};
		level.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, 0.f);
		level.AddComponent<LevelFlyweight>();
	}
}// namespace bomberman
