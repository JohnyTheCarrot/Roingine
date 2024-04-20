#include "scene_impl.h"
#include <roingine/gameobject.h>

namespace roingine {
	GameObject::GameObject(Scene &scene)
	    : m_rpScene{&scene} {
	}

	bool GameObject::operator==(GameObject const &other) const noexcept {
		return m_rpScene == other.m_rpScene && m_hGameObject == other.m_hGameObject;
	}

	GameObjectComponents &GameObject::GetSceneComponents() {
		return m_rpScene->m_pImpl->m_GameObjectComponents;
	}
}// namespace roingine
