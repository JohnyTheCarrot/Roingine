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

	GameObjectComponents &GameObject::GetSceneComponents() noexcept {
		return m_rpScene->m_pImpl->m_GameObjectComponents;
	}

	std::optional<std::size_t> GameObject::GetTypeHashFromName(std::string const &name) const {
		if (!m_rpScene->m_pImpl->m_NameMap.contains(name))
			return std::nullopt;

		return m_rpScene->m_pImpl->m_NameMap.at(name);
	}

	void GameObject::RegisterTypeHashName(std::string name, std::size_t hash) {
		m_rpScene->m_pImpl->m_NameMap.emplace(std::move(name), hash);
	}

	GameObjectComponents const &GameObject::GetSceneComponents() const noexcept {
		return m_rpScene->m_pImpl->m_GameObjectComponents;
	}
}// namespace roingine
