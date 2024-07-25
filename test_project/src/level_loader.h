#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

namespace roingine {
	class Scene;
}

namespace bomberman {
	struct LevelLoadInfo final {
		int windowWidth{};
		int windowHeight{};
	};

	class Level final {
		roingine::Scene *m_rpScene;

		void SpawnPlayer(bool hasKeyboardSupport, int viewX, int viewY, int viewWidth, int viewHeight) const;

	public:
		explicit Level(roingine::Scene &scene);

		void Load(LevelLoadInfo const &loadInfo) const;
	};
}// namespace bomberman

#endif//LEVEL_LOADER_H
