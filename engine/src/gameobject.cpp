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

	Component *GameObject::AddComponent(std::string name, std::vector<JSData> const &args) {
		if (auto *existing{GetOptionalComponent(name)}; existing != nullptr)
			return existing;

		auto const hash{GetTypeHashFromName(name)};
		if (!hash.has_value())
			return nullptr;

		auto const jsFactoryMapEntry{GetJSFactoryMapEntryByHash(hash.value())};
		auto const factory{jsFactoryMapEntry.value().jsFactory};

		std::unique_ptr<Component> pComp{factory(this, args)};
		auto const                 rpComp{pComp.get()};

		ComponentHandle hComponent{m_hGameObject, hash.value()};
		auto           &sceneComponents{GetSceneComponents()};

		sceneComponents.insert({hComponent, std::move(pComp)});
		return rpComp;
	}

	void GameObject::SetLabel(std::string &&label) {
		m_rpScene->m_GameObjectLabels.emplace(m_hGameObject, std::move(label));
	}

	std::optional<std::string_view> GameObject::GetLabel() const {
		auto const it{m_rpScene->m_GameObjectLabels.find(m_hGameObject)};
		if (it == m_rpScene->m_GameObjectLabels.cend())
			return std::nullopt;

		return it->second;
	}

	Component *GameObject::GetComponent(std::size_t typeHash) {
		ComponentHandle hComponent{m_hGameObject, typeHash};
		auto           &sceneComponents{GetSceneComponents()};

		if (sceneComponents.contains(hComponent))
			return sceneComponents.at(hComponent).get();

		throw ComponentNotFoundException{};
	}

	Component *GameObject::GetOptionalComponent(std::size_t typeHash) {
		ComponentHandle hComponent{m_hGameObject, typeHash};
		auto           &sceneComponents{GetSceneComponents()};

		if (sceneComponents.contains(hComponent))
			return sceneComponents.at(hComponent).get();

		return nullptr;
	}

	Component *GameObject::GetComponent(std::string const &name) {
		auto const hash{GetTypeHashFromName(name)};
		if (!hash.has_value())
			return nullptr;

		return GetComponent(hash.value());
	}

	Component *GameObject::GetOptionalComponent(std::string const &name) {
		auto const hash{GetTypeHashFromName(name)};
		if (!hash.has_value())
			return nullptr;

		return GetOptionalComponent(hash.value());
	}

	void GameObject::Destroy() {
		m_rpScene->m_GameObjectsToDestroy.emplace_back(*this);
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

	std::size_t GameObjectHash::operator()(GameObject gameObject) const {
		return reinterpret_cast<size_t>(gameObject.GetScene()) ^ gameObject.GetHandle();
	}
}// namespace roingine
