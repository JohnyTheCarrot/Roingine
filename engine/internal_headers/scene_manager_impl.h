#ifndef SCENE_MANAGER_IMPL_H
#define SCENE_MANAGER_IMPL_H

#include <optional>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>

namespace roingine {
	class SceneManager::Impl final {
	public:
		void SetActive(Scene &&scene);

		[[nodiscard]]
		Scene *GetActive();

		void PreUpdate();

		void Update();

		void PostUpdate();

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
