#ifndef SCENE_LOADER_IMPL_H
#define SCENE_LOADER_IMPL_H

#include <filesystem>
#include <nlohmann/json.hpp>
#include <roingine/scene.h>
#include <roingine/scene_loader.h>

using Json = nlohmann::json;

namespace roingine {
	void from_json(Json const &json, ComponentData &data);

	void from_json(Json const &json, SceneFile &data);

	void from_json(Json const &json, GameObjectData &data);

	class SceneLoader final {
	public:
		SceneLoader(std::filesystem::path filePath);

		Scene operator()();

		void CreateGameObject(GameObjectData const &data);

	private:
		nlohmann::json        m_Data;
		Scene                 m_Scene;
		std::filesystem::path m_Path;
	};
}// namespace roingine

#endif// SCENE_LOADER_IMPL_H
