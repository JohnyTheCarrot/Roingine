#include "scene_impl.h"
#include <roingine/gameobject.h>

namespace roingine {
	GameObject::GameObject(Scene &scene, GameObject::Handle hGoHandle)
	    : m_rpScene{&scene}
	    , m_hGameObject{hGoHandle} {
	}

	bool GameObject::operator==(GameObject const &other) const noexcept {
		return m_rpScene == other.m_rpScene && m_hGameObject == other.m_hGameObject;
	}

	GameObject::Handle GameObject::GetHandle() const noexcept {
		return m_hGameObject;
	}

	void GameObject::SetScene(Scene *scene) noexcept {
		m_rpScene = scene;
	}

	Scene *GameObject::GetScene() const noexcept {
		return m_rpScene;
	}

	GameObjectComponents &GameObject::GetSceneComponents() noexcept {
		return m_rpScene->m_pImpl->m_GameObjectComponents;
	}

	std::optional<std::size_t> GameObject::GetTypeHashFromName(std::string const &name) const {
		if (!m_rpScene->m_pImpl->m_NameMap.contains(name))
			return std::nullopt;

		return m_rpScene->m_pImpl->m_NameMap.at(name);
	}

	std::optional<JSFactoryMapEntry> GameObject::GetJSFactoryMapEntryByHash(std::size_t hash) const {
		if (!m_rpScene->m_pImpl->m_JSFactoryMap.contains(hash))
			return std::nullopt;

		return m_rpScene->m_pImpl->m_JSFactoryMap.at(hash);
	}

	GameObjectComponents const &GameObject::GetSceneComponents() const noexcept {
		return m_rpScene->m_pImpl->m_GameObjectComponents;
	}
}// namespace roingine
