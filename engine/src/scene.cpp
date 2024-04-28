#include "scene_impl.h"
#include <roingine/gameobject.h>

namespace roingine {
	void Scene::Impl::Update() {
		for (auto &component: m_GameObjectComponents) { component.second->Update(); }
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

	void Scene::Impl::AddGameObject(GameObject gameObject) {
		m_GameObjects.push_front(gameObject);
	}

	void Scene::Impl::RemoveGameObject(GameObject gameObject) {
		m_GameObjects.remove(gameObject);
	}

	Scene::Scene()
	    : m_pImpl{std::make_unique<Scene::Impl>()} {
	}

	Scene::~Scene() = default;

	Scene::Scene(Scene &&other) = default;

	Scene &Scene::operator=(Scene &&other) = default;

	void Scene::Update() {
		m_pImpl->Update();
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
}// namespace roingine
