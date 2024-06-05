#include "scene_loader_impl.h"
#include <format>
#include <fstream>
#include <roingine/components/scripts.h>
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

		for (auto &object: sceneFile.objects) { CreateGameObject(object); }

		return std::move(m_Scene);
	}

	void SceneLoader::CreateGameObject(GameObjectData &data) {
		auto gameObject{m_Scene.AddGameObject()};
		if (data.label.has_value())
			gameObject.SetLabel(std::move(data.label.value()));

		if (data.uniqueID.has_value()) {
			m_UniqueIDs.emplace(data.uniqueID.value(), gameObject.GetHandle());
			gameObject.SetUniqueID(std::move(data.uniqueID.value()));
		}

		gameObject.SetEnabled(data.isEnabled);

		for (auto &componentData: data.components) {
			std::for_each(m_UniqueIDs.cbegin(), m_UniqueIDs.cend(), [&](auto const &it) {
				auto const &[uniqueID, handle] = it;

				std::replace_if(
				        componentData.args.begin(), componentData.args.end(),
				        [&](auto &arg) {
					        return std::holds_alternative<std::string>(arg) && std::get<std::string>(arg) == uniqueID;
				        },
				        static_cast<double>(handle)
				);
			});

			if (componentData.name == "Script") {
				auto *scripts{gameObject.GetOptionalComponent<Scripts>()};
				if (!scripts)
					scripts = &gameObject.AddComponent<Scripts>();

				auto                fileName{comp_init::RequireString(0, componentData.args)};
				std::vector<JSData> factoryArgs(std::max(componentData.args.size() - 1, static_cast<std::size_t>(0)));
				std::move(std::next(componentData.args.begin()), componentData.args.end(), factoryArgs.begin());

				scripts->AddScript(fileName, factoryArgs);

				continue;
			}

			gameObject.AddComponent(componentData.name, componentData.args);
		}
	}

	void from_json(Json const &json, ComponentData &data) {
		json.at("name").get_to(data.name);

		auto const args{json.at("args")};
		if (args.type() != Json::value_t::array)
			throw std::runtime_error{"Component args must be an array"};

		data.args.resize(args.size());

		std::transform(args.cbegin(), args.cend(), data.args.begin(), [](Json const &el) -> JSData {
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
		if (json.contains("label")) {
			std::string label{};
			json.at("label").get_to(label);
			data.label = std::move(label);
		}
		if (json.contains("uid")) {
			std::string uniqueID{};
			json.at("uid").get_to(uniqueID);
			data.uniqueID = std::move(uniqueID);
		}
		if (json.contains("isEnabled")) {
			json.at("isEnabled").get_to(data.isEnabled);
		}
		json.at("components").get_to(data.components);
	}

	InvalidComponentArgType::InvalidComponentArgType(std::string_view typeName)
	    : m_Message{std::format("Invalid component arg type {} in scene file.", typeName)} {
	}

	char const *InvalidComponentArgType::what() const {
		return m_Message.c_str();
	}
}// namespace roingine
