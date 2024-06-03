#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <roingine/gameobject.h>
#include <unordered_set>

using duk_context = struct duk_hthread;

namespace roingine {
	class Scene final {
	public:
		Scene();
		~Scene();

		Scene(Scene &&);

		Scene &operator=(Scene &&);

		Scene(Scene const &) = delete;

		Scene &operator=(Scene const &) = delete;

		void PreUpdate();

		void Update();

		void PostUpdate();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		GameObject AddGameObject();

		void RegisterComponentType(std::string name, std::size_t hash, JSFactoryMapEntry::Function jsFactory);

		void SetGameObjectScenes();

		[[nodiscard]]
		GameObjectComponents &GetGameObjectComponents() noexcept;

		[[nodiscard]]
		GameObjectComponents const &GetGameObjectComponents() const noexcept;

		[[nodiscard]]
		std::optional<std::size_t> GetTypeHashFromName(std::string const &name) const;

		[[nodiscard]]
		std::optional<JSFactoryMapEntry> GetJSFactoryMapEntryByHash(std::size_t hash) const;

		template<ComponentImpl TComponent>
		void ForEveryComponentOfType(std::function<void(TComponent &)> const &fn) {
			std::for_each(m_GameObjectComponents.begin(), m_GameObjectComponents.end(), [&fn](auto &pair) {
				auto &[key, pComponent] = pair;
				auto componentHash      = key.second;
				if (componentHash != typeid(TComponent).hash_code())
					return;

				auto &component{*dynamic_cast<TComponent *>(pComponent.get())};
				fn(component);
			});
		}

	private:
		friend class GameObject;

		GameObjectComponents                               m_GameObjectComponents;
		std::unordered_map<std::string, std::size_t>       m_NameMap;
		std::unordered_map<std::size_t, JSFactoryMapEntry> m_JSFactoryMap;
		std::unordered_set<GameObject, GameObjectHash>     m_GameObjects;
		std::vector<GameObject>                            m_GameObjectsToDestroy{};

		void CleanupMarkedGameObjects();

		void AddGameObject(GameObject gameObject);

		void RemoveGameObject(GameObject gameObject);
	};
}// namespace roingine

#endif// SCENE_H
