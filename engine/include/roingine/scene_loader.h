#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <filesystem>
#include <roingine/component_init_types.h>
#include <roingine/scene.h>
#include <string>

namespace roingine {
	struct ComponentData final {
		std::string name;
		std::vector<JSData> args;
	};

	struct GameObjectData final {
		std::optional<std::string> label{std::nullopt};
		std::vector<ComponentData> components;
	};

	struct SceneFile final {
		std::string                 name;
		std::vector<GameObjectData> objects;
	};

	class InvalidComponentArgType final : public std::exception {
	public:
		InvalidComponentArgType(std::string_view typeName);

		char const *what() const override;

	private:
		std::string m_Message;
	};

	namespace scene_loader {
		Scene LoadScene(std::filesystem::path filePath);
	}
}// namespace roingine

#endif// SCENE_LOADER_H
