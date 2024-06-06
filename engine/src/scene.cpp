#include <roingine/components/rect.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/scripts.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace roingine {
	void Scene::PreUpdate() {
		for (auto &componentList: m_GameObjectComponents) {
			for (auto &component: componentList.second) {
				if (component.second->GetGameObject().GetEnabled())
					component.second->PreUpdate();
			}
		}
	}

	void Scene::Update() {
		for (auto &componentList: m_GameObjectComponents) {
			for (auto &component: componentList.second) {
				if (component.second->GetGameObject().GetEnabled())
					component.second->Update();
			}
		}
	}

	void Scene::PostUpdate() {
		for (auto &componentList: m_GameObjectComponents) {
			for (auto &component: componentList.second) {
				if (component.second->GetGameObject().GetEnabled())
					component.second->PostUpdate();
			}
		}
		CleanupMarkedGameObjects();
	}

	void Scene::FixedUpdate() {
		for (auto &componentList: m_GameObjectComponents) {
			for (auto &component: componentList.second) {
				if (component.second->GetGameObject().GetEnabled())
					component.second->FixedUpdate();
			}
		}
	}

	void Scene::Render() const {
		for (auto &componentList: m_GameObjectComponents) {
			for (auto &component: componentList.second) {
				if (component.second->GetGameObject().GetEnabled())
					component.second->Render();
			}
		}
	}

	GameObject Scene::AddGameObject() {
		static GameObject::Handle currentHandle{0};
		GameObject const          go{*this, currentHandle++};

		AddGameObject(go);

		return go;
	}

	void Scene::RegisterComponentType(std::string name, std::size_t hash, JSFactoryMapEntry::Function jsFactory) {
		m_NameMap.emplace(std::move(name), hash);
		m_JSFactoryMap.emplace(hash, JSFactoryMapEntry{jsFactory});
	}

	void Scene::SetGameObjectScenes() {
		for (auto &go: m_GameObjects) { go.second.gameObject.SetScene(this); }
		for (auto &componentList: m_GameObjectComponents) {
			for (auto &component: componentList.second) {
				component.second->GetGameObject().SetScene(this);
				component.second->SceneChanged(*this);
			}
		}
	}

	GameObjectComponents &Scene::GetGameObjectComponents() noexcept {
		return m_GameObjectComponents;
	}

	GameObjectComponents const &Scene::GetGameObjectComponents() const noexcept {
		return m_GameObjectComponents;
	}

	std::optional<std::size_t> Scene::GetTypeHashFromName(std::string const &name) const {
		if (!m_NameMap.contains(name))
			return std::nullopt;

		return m_NameMap.at(name);
	}

	std::optional<JSFactoryMapEntry> Scene::GetJSFactoryMapEntryByHash(std::size_t hash) const {
		if (!m_JSFactoryMap.contains(hash))
			return std::nullopt;

		return m_JSFactoryMap.at(hash);
	}

	GameObject *Scene::GetGameObjectPtr(std::size_t handle) {
		auto it{m_GameObjects.find(handle)};

		if (it != m_GameObjects.end())
			return &m_GameObjects.at(handle).gameObject;

		return nullptr;
	}

	void Scene::CleanupMarkedGameObjects() {
		for (GameObjectHandle gameObject: m_GameObjectsToDestroy) { RemoveGameObject(gameObject); }
		m_GameObjectsToDestroy.clear();
	}

	void Scene::AddGameObject(GameObject gameObject) {
		m_GameObjects.emplace(gameObject.GetHandle(), GameObjectData{.gameObject = gameObject});
	}

	void Scene::RemoveGameObject(GameObjectHandle handle) {
		m_GameObjectComponents.erase(handle);
		m_GameObjects.erase(handle);
	}

	Scene::Scene() {
		RegisterComponentType(Rect::NAME, typeid(Rect).hash_code(), Rect::JSFactory);
		RegisterComponentType(RectRenderer::NAME, typeid(RectRenderer).hash_code(), RectRenderer::JSFactory);
		RegisterComponentType(Transform::NAME, typeid(Transform).hash_code(), Transform::JSFactory);
		RegisterComponentType(Scripts::NAME, typeid(Scripts).hash_code(), Scripts::JSFactory);
		RegisterComponentType(RectCollider::NAME, typeid(RectCollider).hash_code(), RectCollider::JSFactory);
	}

	Scene::~Scene() = default;

	Scene::Scene(Scene &&other)
	    : m_GameObjectComponents{std::move(other.m_GameObjectComponents)}
	    , m_NameMap{std::move(other.m_NameMap)}
	    , m_JSFactoryMap{std::move(other.m_JSFactoryMap)}
	    , m_GameObjects{std::move(other.m_GameObjects)} {
		SetGameObjectScenes();
	};

	Scene &Scene::operator=(Scene &&other) {
		if (this == &other) {
			return *this;
		}

		m_GameObjectComponents = std::move(other.m_GameObjectComponents);
		m_NameMap              = std::move(other.m_NameMap);
		m_JSFactoryMap         = std::move(other.m_JSFactoryMap);
		m_GameObjects          = std::move(other.m_GameObjects);

		SetGameObjectScenes();

		return *this;
	}
}// namespace roingine
