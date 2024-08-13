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
	constexpr int c_Player1StartX{1};
	constexpr int c_Player1StartY{LevelFlyweight::c_LevelHeight - 2};
	constexpr int c_Player2StartX{1};
	constexpr int c_Player2StartY{1};
	constexpr int c_BreathingRoomTileCount{5};

	Level::PlayerAndCam Level::SpawnPlayer(
	        roingine::Scene &scene, PlayerType playerType, bool isPlayer1, int viewX, int viewY, int viewWidth,
	        int viewHeight, LevelFlyweight &levelFlyweight
	) {
		auto  cameraObject{scene.AddGameObject()};
		auto &cameraTransform{cameraObject.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, 0.f)};

		auto *rpCam{&cameraObject.AddComponent<roingine::Camera>(viewX, viewY, viewWidth, viewHeight)};

		auto const playerStartX{isPlayer1 ? c_Player1StartX : c_Player2StartX};
		auto const playerStartY{isPlayer1 ? c_Player1StartY : c_Player2StartY};

		auto player{scene.AddGameObject()};
		player.AddComponent<roingine::Transform>(levelFlyweight.GridToPosition(playerStartX, playerStartY), 0.f);
		if (playerType == PlayerType::Bomberman)
			player.AddComponent<Bomberman>(levelFlyweight);
		else
			player.AddComponent<Enemy>(levelFlyweight, enemy_type::c_EnemyBalloom);

		auto *rpLayer{&player.AddComponent<Player>(cameraTransform, levelFlyweight, isPlayer1)};

		return PlayerAndCam{rpLayer, rpCam};
	}

	Level::Level(LevelLoadInfo const &loadInfo)
	    : m_LoadInfo{loadInfo}
	    , m_rpLevelFlyweight{[this, &loadInfo] {
		    auto level{m_pScene->AddGameObject()};
		    level.AddComponent<roingine::Transform>(glm::vec2{0.f, 0.f}, 0.f);
		    return &level.AddComponent<LevelFlyweight>(loadInfo.setupData.upgrade);
	    }()}
	    , m_rpPlayer1{SpawnPlayer(
	              *m_pScene, PlayerType::Bomberman, true, 0, 0, loadInfo.windowWidth, loadInfo.windowHeight,
	              *m_rpLevelFlyweight
	      )} {
		for (int x{c_Player1StartX}; x < c_Player1StartX + c_BreathingRoomTileCount; ++x)
			m_rpLevelFlyweight->DestroyTile(*m_pScene, x, c_Player1StartY, false);
		for (int x{c_Player2StartX}; x < c_Player2StartX + c_BreathingRoomTileCount; ++x)
			m_rpLevelFlyweight->DestroyTile(*m_pScene, x, c_Player2StartY, false);

		for (int i{}; i < loadInfo.setupData.numBallooms; ++i)
			Enemy::SpawnEnemy(
			        *m_pScene, *m_rpLevelFlyweight, enemy_type::c_EnemyBalloom,
			        m_rpLevelFlyweight->GetRandomEmptyTilePos()
			);
		for (int i{}; i < loadInfo.setupData.numOneals; ++i)
			Enemy::SpawnEnemy(
			        *m_pScene, *m_rpLevelFlyweight, enemy_type::c_EnemyOneal,
			        m_rpLevelFlyweight->GetRandomEmptyTilePos()
			);
		for (int i{}; i < loadInfo.setupData.numDahls; ++i)
			Enemy::SpawnEnemy(
			        *m_pScene, *m_rpLevelFlyweight, enemy_type::c_EnemyDahl, m_rpLevelFlyweight->GetRandomEmptyTilePos()
			);
		for (int i{}; i < loadInfo.setupData.numMinvos; ++i)
			Enemy::SpawnEnemy(
			        *m_pScene, *m_rpLevelFlyweight, enemy_type::c_EnemyMinvo,
			        m_rpLevelFlyweight->GetRandomEmptyTilePos()
			);

		m_Music.Play();
		roingine::SceneManager::GetInstance().SetActive(std::move(*m_pScene));
		m_pScene = nullptr;
	}

	Level::~Level() = default;

	Level::Level(Level &&) noexcept = default;

	Level &Level::operator=(Level &&) noexcept = default;

	void Level::SetPlayer1Controller(roingine::Controller *rpController) const {
		m_rpPlayer1.first->TieToController(rpController);
	}

	void Level::SetUpPlayer2(roingine::Controller &rpController, PlayerType playerType) {
		if (!m_rpPlayer2.has_value()) {
			auto *scene{roingine::SceneManager::GetInstance().GetActive()};

			m_rpPlayer2 = SpawnPlayer(
			        *scene, playerType, false, m_LoadInfo.windowWidth / 2, 0, m_LoadInfo.windowWidth / 2,
			        m_LoadInfo.windowHeight, *m_rpLevelFlyweight
			);
			m_rpPlayer1.second->SetView(0, 0, m_LoadInfo.windowWidth / 2, m_LoadInfo.windowHeight);
		}

		m_rpPlayer2->first->TieToController(&rpController);
	}

	void Level::DisconnectPlayer2() const {
		m_rpPlayer2->first->TieToController(nullptr);
	}

	roingine::Controller *Level::GetPlayer1Controller() const {
		return m_rpPlayer1.first->GetController();
	}

	roingine::Controller *Level::GetPlayer2Controller() const {
		return m_rpPlayer2.has_value() ? m_rpPlayer2->first->GetController() : nullptr;
	}

	std::optional<PlayerType> Level::GetPlayer2Type() const {
		if (!m_rpPlayer2.has_value())
			return std::nullopt;

		if (auto const player{m_rpPlayer2->first->GetGameObject()}; player.GetOptionalComponent<Bomberman>())
			return PlayerType::Bomberman;

		return PlayerType::Balloom;
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
