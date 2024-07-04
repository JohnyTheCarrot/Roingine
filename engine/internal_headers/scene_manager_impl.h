#ifndef SCENE_MANAGER_IMPL_H
#define SCENE_MANAGER_IMPL_H

#include <optional>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>

namespace roingine {
	struct SceneUnload final {};

	struct NoNewScene final {};

	class SceneManager::Impl final {
		void LoadNextImmediately();

	public:
		void SetActive(Scene &&scene);

		void UnloadScene(bool immediate);

		[[nodiscard]]
		Scene *GetActive();

		void PreUpdate();

		void Update();

		void PostUpdate();

		void FixedUpdate();

		void Render() const;

	private:
		std::optional<Scene>                         m_Scene{};
		std::variant<SceneUnload, NoNewScene, Scene> m_NewScene{};
	};
}// namespace roingine

#endif// SCENE_MANAGER_IMPL_H
