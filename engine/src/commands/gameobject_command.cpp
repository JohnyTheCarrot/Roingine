#include <roingine/commands/gameobject_command.h>
#include <roingine/gameobject.h>

namespace roingine {
	GameObjectCommand::GameObjectCommand(GameObject gameObject)
	    : m_GameObject{gameObject} {
	}

	GameObject GameObjectCommand::GetGameObject() const noexcept {
		return m_GameObject;
	}
}// namespace roingine
