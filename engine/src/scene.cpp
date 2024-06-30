#include <roingine/components/rect_collider.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace roingine {
	void Scene::PreUpdate() {
		for (auto &[goHandle, components]: m_GameObjectComponents) {
			for (auto &[compHash, component]: components) {
				if (component->GetGameObject().GetEnabled())
					component->PreUpdate();
			}
		}
	}

	void Scene::Update() {
		for (auto &[goHandle, components]: m_GameObjectComponents) {
			for (auto &[compHash, component]: components) {
				if (component->GetGameObject().GetEnabled())
					component->Update();
			}
		}
	}

	void Scene::PostUpdate() {
		for (auto &[goHandle, components]: m_GameObjectComponents) {
			for (auto &[compHash, component]: components) {
				if (component->GetGameObject().GetEnabled())
					component->PostUpdate();
			}
		}
		CleanupMarkedGameObjects();
	}

	void Scene::FixedUpdate() {
		for (auto &[goHandle, components]: m_GameObjectComponents) {
			for (auto &[compHash, component]: components) {
				if (component->GetGameObject().GetEnabled())
					component->FixedUpdate();
			}
		}
	}

	void Scene::Render() const {
		for (auto &[goHandle, components]: m_GameObjectComponents) {
			for (auto &[compHash, component]: components) {
				if (component->GetGameObject().GetEnabled())
					component->Render();
			}
		}
	}

	GameObject Scene::AddGameObject() {
		static GameObject::Handle currentHandle{0};
		GameObject const          go{*this, currentHandle++};

		AddGameObject(go);

		return go;
	}

	void Scene::SetGameObjectScenes() {
		for (auto &[handle, gameObjectData]: m_GameObjects) { gameObjectData.gameObject.SetScene(this); }
		for (auto &[gameObjectHandle, componentList]: m_GameObjectComponents) {
			for (auto &[compHash, pComponent]: componentList) { pComponent->GetGameObject().SetScene(this); }
		}
	}

	GameObjectComponents &Scene::GetGameObjectComponents() noexcept {
		return m_GameObjectComponents;
	}

	GameObjectComponents const &Scene::GetGameObjectComponents() const noexcept {
		return m_GameObjectComponents;
	}

	GameObject *Scene::GetGameObjectPtr(std::size_t handle) {
		if (auto const it{m_GameObjects.find(handle)}; it != m_GameObjects.end())
			return &m_GameObjects.at(handle).gameObject;

		return nullptr;
	}

	void Scene::CleanupMarkedGameObjects() {
		for (GameObjectHandle const gameObject: m_GameObjectsToDestroy) { RemoveGameObject(gameObject); }
		m_GameObjectsToDestroy.clear();
	}

	void Scene::AddGameObject(GameObject gameObject) {
		m_GameObjects.emplace(gameObject.GetHandle(), GameObjectData{.gameObject = gameObject});
	}

	void Scene::RemoveGameObject(GameObjectHandle handle) {
		m_GameObjectComponents.erase(handle);
		m_GameObjects.erase(handle);
	}

	Scene::Scene() = default;

	Scene::~Scene() = default;

	Scene::Scene(Scene &&other) noexcept
	    : m_GameObjectComponents{std::move(other.m_GameObjectComponents)}
	    , m_GameObjects{std::move(other.m_GameObjects)} {
		SetGameObjectScenes();
	};

	Scene &Scene::operator=(Scene &&other) noexcept {
		if (this == &other) {
			return *this;
		}

		m_GameObjectComponents = std::move(other.m_GameObjectComponents);
		m_GameObjects          = std::move(other.m_GameObjects);

		SetGameObjectScenes();

		return *this;
	}
}// namespace roingine
