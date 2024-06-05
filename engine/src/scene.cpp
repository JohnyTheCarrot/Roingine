#include <roingine/components/rect.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/scripts.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace roingine {
	void Scene::PreUpdate() {
		for (auto &component: m_GameObjectComponents) { component.second->PreUpdate(); }
	}

	void Scene::Update() {
		for (auto &component: m_GameObjectComponents) { component.second->Update(); }
	}

	void Scene::PostUpdate() {
		for (auto &component: m_GameObjectComponents) { component.second->PostUpdate(); }
		CleanupMarkedGameObjects();
	}

	void Scene::FixedUpdate() {
		for (auto &component: m_GameObjectComponents) { component.second->FixedUpdate(); }
	}

	void Scene::Render() const {
		for (auto const &component: m_GameObjectComponents) { component.second->Render(); }
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
		for (auto &go: m_GameObjects) { go.second.SetScene(this); }
		for (auto &comp: m_GameObjectComponents) {
			comp.second->GetGameObject().SetScene(this);
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
		if (m_GameObjects.contains(handle))
			return &m_GameObjects.at(handle);

		return nullptr;
	}

	void Scene::CleanupMarkedGameObjects() {
		for (GameObject gameObject: m_GameObjectsToDestroy) { RemoveGameObject(gameObject); }
		m_GameObjectsToDestroy.clear();
	}

	void Scene::AddGameObject(GameObject gameObject) {
		m_GameObjects.emplace(gameObject.GetHandle(), gameObject);
	}

	void Scene::RemoveGameObject(GameObject gameObject) {
		std::erase_if(m_GameObjectComponents, [&](auto &it) {
			auto &[key, value]  = it;
			auto currGameObject = key.first;

			return currGameObject == gameObject.GetHandle();
		});

		m_GameObjects.erase(gameObject.GetHandle());
		m_GameObjectLabels.erase(gameObject.GetHandle());
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
	    , m_GameObjects{std::move(other.m_GameObjects)}
	    , m_GameObjectLabels{std::move(other.m_GameObjectLabels)} {
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
		m_GameObjectLabels     = std::move(other.m_GameObjectLabels);

		SetGameObjectScenes();

		return *this;
	}
}// namespace roingine
