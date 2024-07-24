#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
#include <vector>

namespace roingine {
	class Scene;
}

namespace bomberman::level_loader {
	struct LevelLoadInfo final {};

	void LoadScene(roingine::Scene &scene, LevelLoadInfo const &levelLoadInfo);
}// namespace bomberman::level_loader

#endif//LEVEL_LOADER_H
