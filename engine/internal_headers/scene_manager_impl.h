#ifndef SCENE_MANAGER_IMPL_H
#define SCENE_MANAGER_IMPL_H

#include "scene.h"
#include "scene_manager.h"
#include <optional>

namespace roingine {
	class SceneManager::Impl final {
	public:
		void SetActive(Scene &&scene);

		void Update();

		void FixedUpdate();

		void Render() const;

	private:
		std::optional<Scene> m_Scene{};
	};

	SceneManager::SceneManager()
	    : m_pImpl{std::make_unique<SceneManager::Impl>()} {
	}

	SceneManager::~SceneManager() = default;
}// namespace roingine

#endif// SCENE_MANAGER_IMPL_H
