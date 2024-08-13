#include "level.h"

#include "components/bomberman.h"
#include "components/enemy.h"
#include "components/level_flyweight.h"
#include "components/player.h"

#include <roingine/components/camera.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>

namespace bomberman {
	constexpr int c_PlayerStartX{1};
	constexpr int c_PlayerStartY{LevelFlyweight::c_LevelHeight - 2};

	Level::PlayerAndCam Level::SpawnPlayer(
	        roingine::Scene &scene, bool hasKeyboardSupport, int viewX, int viewY, int viewWidth, int viewHeight,
	        LevelFlyweight const &levelFlyweight
	) {
		auto  cameraObject{scene.AddGameObject()};
		auto &cameraTransform{cameraObject.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, 0.f)};

		auto *rpCam{&cameraObject.AddComponent<roingine::Camera>(viewX, viewY, viewWidth, viewHeight)};

		auto player{scene.AddGameObject()};
		player.AddComponent<roingine::Transform>(
		        glm::vec2{c_PlayerStartX * LevelFlyweight::c_TileSize, c_PlayerStartY * LevelFlyweight::c_TileSize}, 0.f
		);
		player.AddComponent<Bomberman>(levelFlyweight);
		auto *rpLayer{&player.AddComponent<Player>(cameraTransform, levelFlyweight, hasKeyboardSupport)};

		return PlayerAndCam{rpLayer, rpCam};
	}

	Level::Level(roingine::Scene &scene, LevelLoadInfo const &loadInfo)
	    : m_LoadInfo{loadInfo}
	    , m_rpLevelFlyweight{[this, &scene] {
		    auto level{scene.AddGameObject()};
		    level.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, 0.f);
		    return &level.AddComponent<LevelFlyweight>();
	    }()}
	    , m_rpPlayer1{SpawnPlayer(scene, true, 0, 0, loadInfo.windowWidth, loadInfo.windowHeight, *m_rpLevelFlyweight)
	      } {
		for (int i{}; i < loadInfo.numBallooms; ++i)
			Enemy::SpawnEnemy(
			        scene, *m_rpLevelFlyweight, enemy_type::c_EnemyBalloom, m_rpLevelFlyweight->GetRandomEmptyTilePos()
			);
		m_Music.Play();
		roingine::SceneManager::GetInstance().SetActive(std::move(scene));
	}

	void Level::SetPlayer1Controller(roingine::Controller *rpController) const {
		m_rpPlayer1.first->TieToController(rpController);
	}

	void Level::SetPlayer2Controller(roingine::Controller *rpController) {
		if (!m_rpPlayer2.has_value()) {
			auto *scene{roingine::SceneManager::GetInstance().GetActive()};

			m_rpPlayer2 = SpawnPlayer(
			        *scene, false, m_LoadInfo.windowWidth / 2, 0, m_LoadInfo.windowWidth / 2, m_LoadInfo.windowHeight,
			        *m_rpLevelFlyweight
			);
			m_rpPlayer1.second->SetView(0, 0, m_LoadInfo.windowWidth / 2, m_LoadInfo.windowHeight);
		}

		m_rpPlayer2->first->TieToController(rpController);
	}

	bool Level::DoesPlayer1OwnController(roingine::Controller const &controller) const {
		return m_rpPlayer1.first->HasSpecificController(&controller);
	}

	bool Level::DoesPlayer2OwnController(roingine::Controller const &controller) const {
		return m_rpPlayer2.has_value() && m_rpPlayer2->first->HasSpecificController(&controller);
	}

	bool Level::DoesPlayer1HaveController() const {
		return m_rpPlayer1.first->HasConnectedController();
	}

	bool Level::DoesPlayer2HaveController() const {
		return m_rpPlayer2.has_value() && m_rpPlayer2->first->HasConnectedController();
	}
}// namespace bomberman
