#include "scene_impl.h"
#include <roingine/components/rect.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/scripts.h>
#include <roingine/components/transform.h>
#include <roingine/gameobject.h>

namespace roingine {
	void Scene::Impl::PreUpdate() {
		for (auto &component: m_GameObjectComponents) { component.second->PreUpdate(); }
	}

	void Scene::Impl::Update() {
		for (auto &component: m_GameObjectComponents) { component.second->Update(); }
	}

	void Scene::Impl::PostUpdate() {
		for (auto &component: m_GameObjectComponents) { component.second->PostUpdate(); }
	}

	void Scene::Impl::FixedUpdate() {
		for (auto &component: m_GameObjectComponents) { component.second->FixedUpdate(); }
	}

	void Scene::Impl::Render() const {
		for (auto const &component: m_GameObjectComponents) { component.second->Render(); }
	}

	GameObject Scene::Impl::AddGameObject(Scene &scene) {
		static GameObject::Handle currentHandle{0};
		GameObject const          go{scene, currentHandle++};

		AddGameObject(go);

		return go;
	}

	void Scene::Impl::RegisterComponentType(std::string name, std::size_t hash, JSFactoryMapEntry::Function jsFactory) {
		m_NameMap.emplace(std::move(name), hash);
		m_JSFactoryMap.emplace(hash, JSFactoryMapEntry{jsFactory});
	}

	void Scene::Impl::SetGameObjectScenes(Scene &scene) {
		for (auto &go: m_GameObjects) { go.SetScene(&scene); }
		for (auto &comp: m_GameObjectComponents) { comp.second->GetGameObject().SetScene(&scene); }
	}

	void Scene::Impl::AddGameObject(GameObject gameObject) {
		m_GameObjects.push_front(gameObject);
	}

	void Scene::Impl::RemoveGameObject(GameObject gameObject) {
		m_GameObjects.remove(gameObject);
	}

	Scene::Scene()
	    : m_pImpl{std::make_unique<Scene::Impl>()} {
		RegisterComponentType(Rect::NAME, typeid(Rect).hash_code(), Rect::JSFactory);
		RegisterComponentType(RectRenderer::NAME, typeid(RectRenderer).hash_code(), RectRenderer::JSFactory);
		RegisterComponentType(Transform::NAME, typeid(Transform).hash_code(), Transform::JSFactory);
		RegisterComponentType(Scripts::NAME, typeid(Scripts).hash_code(), Scripts::JSFactory);
	}

	Scene::~Scene() = default;

	Scene::Scene(Scene &&other)
	    : m_pImpl{std::move(other.m_pImpl)} {
		m_pImpl->SetGameObjectScenes(*this);
	};

	Scene &Scene::operator=(Scene &&other) {
		if (this == &other) {
			return *this;
		}

		m_pImpl = std::move(other.m_pImpl);
		m_pImpl->SetGameObjectScenes(*this);

		return *this;
	}

	void Scene::PreUpdate() {
		m_pImpl->PreUpdate();
	}

	void Scene::Update() {
		m_pImpl->Update();
	}

	void Scene::PostUpdate() {
		m_pImpl->PostUpdate();
	}

	void Scene::FixedUpdate() {
		m_pImpl->FixedUpdate();
	}

	void Scene::Render() const {
		m_pImpl->Render();
	}

	GameObject Scene::AddGameObject() {
		return m_pImpl->AddGameObject(*this);
	}

	void Scene::RegisterComponentType(std::string name, std::size_t hash, JSFactoryMapEntry::Function jsFactory) {
		m_pImpl->RegisterComponentType(name, hash, jsFactory);
	}
}// namespace roingine
