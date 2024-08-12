#ifndef BOMB_H
#define BOMB_H

#include <roingine/components/component.h>
#include <roingine/gameobject.h>

namespace bomberman {
	class TemporaryObject final : public roingine::Component {
		using OnDeathFn = std::function<void(roingine::GameObject)>;
		std::optional<OnDeathFn> m_OnDeath{};
		float                    m_Age{0.f};
		float                    m_MaxAge{0.f};

	public:
		explicit
		TemporaryObject(roingine::GameObject gameObject, float maxAge, std::optional<OnDeathFn> onDeath = std::nullopt);

		void Update() override;
	};
}// namespace bomberman

#endif//BOMB_H
