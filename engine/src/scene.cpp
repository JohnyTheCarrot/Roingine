#include <roingine/components/rect.h>
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

	void Scene::SetGameObjectScenes(Scene &scene) {
		for (auto &go: m_GameObjects) { go.SetScene(&scene); }
		for (auto &comp: m_GameObjectComponents) { comp.second->GetGameObject().SetScene(&scene); }
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

	void Scene::AddGameObject(GameObject gameObject) {
		m_GameObjects.push_front(gameObject);
	}

	void Scene::RemoveGameObject(GameObject gameObject) {
		m_GameObjects.remove(gameObject);
	}

	Scene::Scene() {
		RegisterComponentType(Rect::NAME, typeid(Rect).hash_code(), Rect::JSFactory);
		RegisterComponentType(RectRenderer::NAME, typeid(RectRenderer).hash_code(), RectRenderer::JSFactory);
		RegisterComponentType(Transform::NAME, typeid(Transform).hash_code(), Transform::JSFactory);
		RegisterComponentType(Scripts::NAME, typeid(Scripts).hash_code(), Scripts::JSFactory);
	}

	Scene::~Scene() = default;

	Scene::Scene(Scene &&other)
	    : m_GameObjectComponents{std::move(other.m_GameObjectComponents)}
	    , m_NameMap{std::move(other.m_NameMap)}
	    , m_JSFactoryMap{std::move(other.m_JSFactoryMap)}
	    , m_GameObjects{std::move(other.m_GameObjects)} {
		SetGameObjectScenes(*this);
	};

	Scene &Scene::operator=(Scene &&other) {
		if (this == &other) {
			return *this;
		}

		m_GameObjectComponents = std::move(other.m_GameObjectComponents);
		m_NameMap              = std::move(other.m_NameMap);
		m_JSFactoryMap         = std::move(other.m_JSFactoryMap);
		m_GameObjects          = std::move(other.m_GameObjects);

		SetGameObjectScenes(*this);

		return *this;
	}
}// namespace roingine
