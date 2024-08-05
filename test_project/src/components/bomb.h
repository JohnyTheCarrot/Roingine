#ifndef BOMB_H
#define BOMB_H

#include <roingine/components/component.h>
#include <roingine/gameobject.h>

namespace bomberman {
	class Bomb final : public roingine::Component {
		float m_Age{0.f};

	public:
		explicit Bomb(roingine::GameObject gameObject);

		void Update() override;
	};
}// namespace bomberman

#endif//BOMB_H
