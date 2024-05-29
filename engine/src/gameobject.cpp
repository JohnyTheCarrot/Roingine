#include <roingine/gameobject.h>
#include <roingine/scene.h>

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

	// why not call this on scene directly? bc of sircular dependencies
	std::optional<std::size_t> GameObject::GetTypeHashFromName(std::string const &name) const {
		return m_rpScene->GetTypeHashFromName(name);
	}

	std::optional<JSFactoryMapEntry> GameObject::GetJSFactoryMapEntryByHash(std::size_t hash) const {
		return m_rpScene->GetJSFactoryMapEntryByHash(hash);
	}

	GameObjectComponents const &GameObject::GetSceneComponents() const noexcept {
		return m_rpScene->GetGameObjectComponents();
	}

	GameObjectComponents &GameObject::GetSceneComponents() noexcept {
		return m_rpScene->GetGameObjectComponents();
	}
}// namespace roingine
