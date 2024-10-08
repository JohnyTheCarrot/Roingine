#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "upgrade_type.h"

#include <roingine/controller.h>
#include <roingine/gameobject.h>
#include <roingine/music.h>
#include <roingine/scene.h>

namespace roingine {
	class Camera;
}// namespace roingine

namespace bomberman {
	class LevelFlyweight;

	class Player;
}// namespace bomberman

namespace bomberman {
	struct LevelSetupData {
		UpgradeType upgrade{};
		int         numBallooms{0};
		int         numOneals{0};
		int         numDahls{0};
		int         numMinvos{0};
	};

	struct LevelLoadInfo final {
		int            windowWidth{};
		int            windowHeight{};
		LevelSetupData setupData{};
	};

	enum class PlayerType { Bomberman, Balloom };

	class Level final {
		std::unique_ptr<roingine::Scene> m_pScene{std::make_unique<roingine::Scene>()};
		roingine::Music                  m_Music{"res/sound/bg_music.wav"};
		LevelLoadInfo                    m_LoadInfo;
		LevelFlyweight                  *m_rpLevelFlyweight;
		using PlayerAndCam = std::pair<Player *, roingine::Camera *>;
		PlayerAndCam                m_rpPlayer1;
		std::optional<PlayerAndCam> m_rpPlayer2{};

		[[nodiscard]]
		static PlayerAndCam SpawnPlayer(
		        roingine::Scene &scene, PlayerType playerType, bool isPlayer1, int viewX, int viewY, int viewWidth,
		        int viewHeight, LevelFlyweight &levelFlyweight
		);

	public:
		explicit Level(LevelLoadInfo const &loadInfo);

		~Level();

		Level(Level &&) noexcept;

		Level &operator=(Level &&) noexcept;

		Level(Level const &) = delete;

		void SetPlayer1Controller(roingine::Controller *rpController) const;

		void SetUpPlayer2(roingine::Controller &rpController, PlayerType playerType);

		void DisconnectPlayer2() const;

		[[nodiscard]]
		roingine::Controller *GetPlayer1Controller() const;

		[[nodiscard]]
		roingine::Controller *GetPlayer2Controller() const;

		[[nodiscard]]
		std::optional<PlayerType> GetPlayer2Type() const;

		[[nodiscard]]
		bool DoesPlayer1OwnController(roingine::Controller const &controller) const;

		[[nodiscard]]
		bool DoesPlayer2OwnController(roingine::Controller const &controller) const;

		[[nodiscard]]
		bool DoesPlayer1HaveController() const;

		[[nodiscard]]
		bool DoesPlayer2HaveController() const;
	};
}// namespace bomberman

#endif//LEVEL_LOADER_H
