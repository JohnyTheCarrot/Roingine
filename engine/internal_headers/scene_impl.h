#ifndef SCENE_IMPL_H
#define SCENE_IMPL_H

#include <forward_list>
#include <roingine/gameobject_types.h>
#include <roingine/scene.h>

namespace roingine {
	class GameObject;

	class Component;

	class Scene::Impl final {
	public:
		void PreUpdate();

		void Update();

		void PostUpdate();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		GameObject AddGameObject(Scene &scene);

	private:
		friend class GameObject;

		GameObjectComponents          m_GameObjectComponents;
		std::unordered_map<std::string, std::size_t> m_NameMap;
		std::forward_list<GameObject> m_GameObjects;

		void AddGameObject(GameObject gameObject);

		void RemoveGameObject(GameObject gameObject);
	};
}// namespace roingine

#endif// SCENE_IMPL_H
