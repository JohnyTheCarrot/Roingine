#include <ranges>
#include <roingine/components/component.h>
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

	void GameObject::SetLabel(std::string &&label) const {
		m_rpScene->m_GameObjects.at(m_hGameObject).label = std::move(label);
	}

	std::optional<std::string_view> GameObject::GetLabel() const {
		return m_rpScene->m_GameObjects.at(m_hGameObject).label;
	}

	void GameObject::SetUniqueID(std::string &&uniqueID) const {
		m_rpScene->m_GameObjects.at(m_hGameObject).uniqueID = std::move(uniqueID);
	}

	std::optional<std::string_view> GameObject::GetUniqueID() const {
		return m_rpScene->m_GameObjects.at(m_hGameObject).uniqueID;
	}

	void GameObject::SetEnabled(bool enabled) {
		m_rpScene->m_GameObjects.at(m_hGameObject).isEnabled = enabled;

		if (auto *pComponents{GetComponents()}; pComponents != nullptr)
			for (auto const &pComponent: *pComponents | std::views::values) {
				if (enabled)
					pComponent->OnEnabled();
				else
					pComponent->OnDisabled();
			}
	}

	bool GameObject::GetEnabled() const {
		return m_rpScene->m_GameObjects.at(m_hGameObject).isEnabled;
	}

	Component *GameObject::GetComponent(std::size_t typeHash) {
		if (auto const pComp{GetOptionalComponent(typeHash)}; pComp != nullptr)
			return pComp;

		throw ComponentNotFoundException{};
	}

	Component *GameObject::GetOptionalComponent(std::size_t typeHash) {
		ComponentHandle const hComponent{typeHash};

		if (auto const sceneComponents{GetComponents()}; sceneComponents != nullptr) {
			if (auto const it{sceneComponents->find(hComponent)}; it != sceneComponents->end())
				return it->second.get();
		}

		return nullptr;
	}

	void GameObject::Destroy() {
		m_rpScene->m_GameObjectsToDestroy.emplace_back(m_hGameObject);
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

	ComponentMap *GameObject::GetComponents() {
		auto const it{m_rpScene->m_GameObjectComponents.find(m_hGameObject)};
		if (it == m_rpScene->m_GameObjectComponents.end())
			return nullptr;

		return &it->second;
	}

	ComponentMap const *GameObject::GetComponents() const {
		auto const it{m_rpScene->m_GameObjectComponents.find(m_hGameObject)};
		if (it == m_rpScene->m_GameObjectComponents.end())
			return nullptr;

		return &it->second;
	}

	ComponentMap &GameObject::GetOrCreateComponentList() {
		auto const it{m_rpScene->m_GameObjectComponents.find(m_hGameObject)};
		if (it == m_rpScene->m_GameObjectComponents.end()) {
			auto const [gameObjectToCompMap, _]{m_rpScene->m_GameObjectComponents.emplace(m_hGameObject, ComponentMap{})
			};
			return gameObjectToCompMap->second;
		}

		return it->second;
	}

	GameObjectComponents const &GameObject::GetSceneComponents() const noexcept {
		return m_rpScene->GetGameObjectComponents();
	}

	GameObjectComponents &GameObject::GetSceneComponents() noexcept {
		return m_rpScene->GetGameObjectComponents();
	}

	std::size_t GameObjectHash::operator()(GameObject gameObject) const {
		return reinterpret_cast<size_t>(gameObject.GetScene()) ^ gameObject.GetHandle();
	}
}// namespace roingine
