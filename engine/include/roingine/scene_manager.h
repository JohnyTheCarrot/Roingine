#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "singleton.h"
#include <memory>

namespace roingine {
	class Scene;

	class SceneManager final : public Singleton<SceneManager> {
	public:
		SceneManager();

		~SceneManager() override;

		void SetActive(Scene &&scene) const;

		void UnloadScene(bool immediate = false) const;

		[[nodiscard]]
		Scene *GetActive() const;

		void PreUpdate() const;

		void Update() const;

		void PostUpdate() const;

		void FixedUpdate() const;

		void RenderFromCameras() const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// SCENE_MANAGER_H
