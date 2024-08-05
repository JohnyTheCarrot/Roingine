#include "place_bomb_command.h"

#include "components/bomb.h"
#include "roingine/components/rect_renderer.h"

#include <roingine/scene.h>
#include <roingine/scene_manager.h>

namespace bomberman {
	PlaceBombCommand::PlaceBombCommand(roingine::Transform const &rpTransform)
	    : m_rpTransform{&rpTransform} {
	}

	void PlaceBombCommand::Execute() {
		auto const activeScene{roingine::SceneManager::GetInstance().GetActive()};
		auto       bomb{activeScene->AddGameObject()};

		bomb.AddComponent<roingine::Transform>(m_rpTransform->GetWorldPosition(), 0.f);
		bomb.AddComponent<roingine::RectRenderer>(20.f, 20.f);
		bomb.AddComponent<Bomb>();
	}
}// namespace bomberman
