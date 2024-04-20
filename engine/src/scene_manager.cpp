#include "scene_manager_impl.h"

namespace roingine {
	void SceneManager::Impl::SetActive(Scene &&scene) {
		m_Scene = std::move(scene);
	}

	void SceneManager::Impl::Update() {
		if (!m_Scene.has_value())
			return;

		m_Scene->Update();
	}

	void SceneManager::Impl::FixedUpdate() {
		if (!m_Scene.has_value())
			return;

		m_Scene->FixedUpdate();
	}

	void SceneManager::Impl::Render() const {
		if (!m_Scene.has_value())
			return;

		m_Scene->Render();
	}

	void SceneManager::SetActive(Scene &&scene) {
		m_pImpl->SetActive(std::move(scene));
	}

	void SceneManager::Update() {
		m_pImpl->Update();
	}

	void SceneManager::FixedUpdate() {
		m_pImpl->FixedUpdate();
	}

	void SceneManager::Render() const {
		m_pImpl->Render();
	}
}// namespace roingine
