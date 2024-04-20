#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "singleton.h"
#include <memory>

namespace roingine {
	class Scene;

	class SceneManager final : public Singleton<SceneManager> {
	public:
		SceneManager();
		~SceneManager();

		void SetActive(Scene &&scene);

		void Update();

		void FixedUpdate();

		void Render() const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// SCENE_MANAGER_H
