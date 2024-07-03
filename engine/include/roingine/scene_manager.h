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

		void SetActive(Scene &&scene);

		[[nodiscard]]
		Scene *GetActive();

		void PreUpdate();

		void Update();

		void PostUpdate();

		void FixedUpdate();

		void RenderFromCameras() const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// SCENE_MANAGER_H
