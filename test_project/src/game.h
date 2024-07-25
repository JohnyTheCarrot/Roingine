#ifndef GAME_H
#define GAME_H

#include <roingine/gameobject.h>

namespace roingine {
	class Camera;

	class Transform;
}// namespace roingine

namespace bomberman {
	class Game final {
		roingine::Scene *m_rpScene{};

	public:
		Game(roingine::Scene &&scene, int windowWidth, int windowHeight);
	};
}// namespace bomberman

#endif//GAME_H
