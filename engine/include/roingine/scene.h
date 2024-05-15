#ifndef SCENE_H
#define SCENE_H

#include "gameobject.h"
#include <memory>

namespace roingine {
	class Scene final {
	public:
		Scene();
		~Scene();

		Scene(Scene &&);

		Scene &operator=(Scene &&);

		void PreUpdate();

		void Update();

		void PostUpdate();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		GameObject AddGameObject();

	private:
		friend class GameObject;

		// TODO: pImpl seems unnecessary here, remove it
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// SCENE_H
