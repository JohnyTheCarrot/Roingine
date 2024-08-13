#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include <roingine/controller.h>
#include <roingine/gameobject.h>
#include <roingine/music.h>

namespace roingine {
	class Camera;

	class Scene;
}// namespace roingine

namespace bomberman {
	class LevelFlyweight;

	class Player;
}// namespace bomberman

namespace bomberman {
	struct LevelLoadInfo final {
		int windowWidth{};
		int windowHeight{};
		int numBallooms{0};
	};

	class Level final {
		roingine::Music     m_Music{"res/sound/bg_music.wav"};
		LevelLoadInfo const m_LoadInfo;
		LevelFlyweight     *m_rpLevelFlyweight;
		using PlayerAndCam = std::pair<Player *, roingine::Camera *>;
		PlayerAndCam                m_rpPlayer1;
		std::optional<PlayerAndCam> m_rpPlayer2{};

		[[nodiscard]]
		static PlayerAndCam SpawnPlayer(
		        roingine::Scene &scene, bool hasKeyboardSupport, int viewX, int viewY, int viewWidth, int viewHeight,
		        LevelFlyweight const &levelFlyweight
		);

	public:
		Level(roingine::Scene &scene, LevelLoadInfo const &loadInfo);

		void SetPlayer1Controller(roingine::Controller *rpController) const;

		void SetPlayer2Controller(roingine::Controller *rpController);

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
