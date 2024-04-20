#ifndef SCENE_IMPL_H
#define SCENE_IMPL_H

#include "gameobject_types.h"
#include "scene.h"
#include <forward_list>

namespace roingine {
	class GameObject;

	class Component;

	class Scene::Impl final {
	public:
		void Update();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		GameObject AddGameObject(Scene &scene);

	private:
		friend class GameObject;

		GameObjectComponents          m_GameObjectComponents;
		std::forward_list<GameObject> m_GameObjects;

		void AddGameObject(GameObject gameObject);

		void RemoveGameObject(GameObject gameObject);
	};
}// namespace roingine

#endif// SCENE_IMPL_H
