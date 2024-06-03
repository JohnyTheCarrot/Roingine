#include "scene_loader_impl.h"
#include <format>
#include <fstream>
#include <roingine/scene_loader.h>

namespace roingine {
	namespace scene_loader {
		Scene LoadScene(std::filesystem::path filePath) {
			SceneLoader sceneLoader{std::move(filePath)};

			return sceneLoader();
		}
	}// namespace scene_loader

	SceneLoader::SceneLoader(std::filesystem::path filePath)
	    : m_Scene{}
	    , m_Path{std::move(filePath)} {
		std::ifstream ifstream{m_Path};
		m_Data = Json::parse(ifstream);
	}

	Scene SceneLoader::operator()() {
		auto sceneFile{m_Data.template get<SceneFile>()};

		for (auto const &object: sceneFile.objects) { CreateGameObject(object); }

		return std::move(m_Scene);
	}

	void SceneLoader::CreateGameObject(GameObjectData const &data) {
		auto gameObject{m_Scene.AddGameObject()};

		for (auto const &componentData: data.components) {
			gameObject.AddComponent(componentData.name, componentData.args);
		}
	}

	void from_json(Json const &json, ComponentData &data) {
		json.at("name").get_to(data.name);

		auto const args{json.at("args")};
		if (args.type() != Json::value_t::array)
			throw std::runtime_error{"Component args must be an array"};

		data.args.resize(args.size());

		std::transform(args.cbegin(), args.cend(), data.args.begin(), [](Json const &el) -> ComponentInitArgument {
			switch (el.type()) {
				case Json::value_t::string:
					return el.get<std::string>();
				case Json::value_t::number_integer:
				case Json::value_t::number_unsigned:
				case Json::value_t::number_float:
					return el.get<double>();
				case Json::value_t::boolean:
					return el.get<bool>();
				default:
					throw InvalidComponentArgType{el.type_name()};
			}
		});
	}

	void from_json(Json const &json, SceneFile &data) {
		json.at("name").get_to(data.name);
		json.at("objects").get_to(data.objects);
	}

	void from_json(Json const &json, GameObjectData &data) {
		json.at("components").get_to(data.components);
	}

	InvalidComponentArgType::InvalidComponentArgType(std::string_view typeName)
	    : m_Message{std::format("Invalid component arg type {} in scene file.", typeName)} {
	}

	char const *InvalidComponentArgType::what() const {
		return m_Message.c_str();
	}
}// namespace roingine
