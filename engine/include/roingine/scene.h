#ifndef SCENE_H
#define SCENE_H

#include <forward_list>
#include <roingine/gameobject.h>

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

		void SetGameObjectScenes(Scene &scene);

		[[nodiscard]]
		GameObjectComponents &GetGameObjectComponents() noexcept;

		[[nodiscard]]
		GameObjectComponents const &GetGameObjectComponents() const noexcept;

		[[nodiscard]]
		std::optional<std::size_t> GetTypeHashFromName(std::string const &name) const;

		[[nodiscard]]
		std::optional<JSFactoryMapEntry> GetJSFactoryMapEntryByHash(std::size_t hash) const;

	private:
		friend class GameObject;

		GameObjectComponents                               m_GameObjectComponents;
		std::unordered_map<std::string, std::size_t>       m_NameMap;
		std::unordered_map<std::size_t, JSFactoryMapEntry> m_JSFactoryMap;
		std::forward_list<GameObject>                      m_GameObjects;

		void AddGameObject(GameObject gameObject);

		void RemoveGameObject(GameObject gameObject);
	};
}// namespace roingine

#endif// SCENE_H
