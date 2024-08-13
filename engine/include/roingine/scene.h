#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <roingine/gameobject.h>
#include <unordered_map>

using duk_context = struct duk_hthread;

namespace roingine {
	class Scene final {
	public:
		 Scene();
		~Scene();

		Scene(Scene &&) noexcept;

		Scene &operator=(Scene &&) noexcept;

		Scene(Scene const &) = delete;

		Scene &operator=(Scene const &) = delete;

		void PreUpdate();

		void Update();

		void PostUpdate();

		void FixedUpdate();

		void Render() const;

		void RenderFromCameras() const;

		[[nodiscard]]
		GameObject AddGameObject();

		void SetGameObjectScenes();

		[[nodiscard]]
		GameObjectComponents &GetGameObjectComponents() noexcept;

		[[nodiscard]]
		GameObjectComponents const &GetGameObjectComponents() const noexcept;

		template<class TComponent>
		void ForEveryComponentOfType(std::function<void(TComponent &)> const &fn) {
			std::for_each(m_GameObjectComponents.begin(), m_GameObjectComponents.end(), [&fn](auto &goCompPair) {
				std::for_each(goCompPair.second.begin(), goCompPair.second.end(), [&fn](auto &pair) {
					auto &[componentHash, pComponent] = pair;
					if (componentHash != typeid(TComponent).hash_code())
						return;

					auto &component{*dynamic_cast<TComponent *>(pComponent.get())};
					fn(component);
				});
			});
		}

		template<class TComponent>
		[[nodiscard]]
		int CountComponentsOfType() const {
			int count{};
			std::for_each(m_GameObjectComponents.begin(), m_GameObjectComponents.end(), [&count](auto &goCompPair) {
				std::for_each(goCompPair.second.begin(), goCompPair.second.end(), [&count](auto &pair) {
					auto &[componentHash, pComponent] = pair;
					if (componentHash != typeid(TComponent).hash_code())
						return;

					++count;
				});
			});

			return count;
		}

		[[nodiscard]]
		GameObject *GetGameObjectPtr(std::size_t handle);

	private:
		struct GameObjectData final {
			std::optional<std::string> uniqueID{};
			std::optional<std::string> label{};
			GameObject                 gameObject;
			bool                       isEnabled{true};
		};

		friend class GameObject;

		GameObjectComponents                            m_GameObjectComponents;
		std::unordered_map<std::size_t, GameObjectData> m_GameObjects;
		std::vector<GameObjectHandle>                   m_GameObjectsToDestroy{};

		void CleanupMarkedGameObjects();

		void AddGameObject(GameObject gameObject);

		void RemoveGameObject(GameObjectHandle handle);
	};
}// namespace roingine

#endif// SCENE_H
