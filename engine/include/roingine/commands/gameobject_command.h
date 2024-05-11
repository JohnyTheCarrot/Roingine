#ifndef GAMEOBJECT_COMMAND_H
#define GAMEOBJECT_COMMAND_H

#include <roingine/commands/command.h>
#include <roingine/gameobject.h>

namespace roingine {
	class GameObjectCommand : public Command {
	public:
		explicit GameObjectCommand(GameObject gameObject);

		~GameObjectCommand() override = default;

	protected:
		[[nodiscard]]
		GameObject GetGameObject() const noexcept;

	private:
		GameObject m_GameObject;
	};
}// namespace roingine

#endif// GAMEOBJECT_COMMAND_H
