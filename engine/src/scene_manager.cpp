#include "scene_manager_impl.h"

namespace roingine {
	void SceneManager::Impl::SetActive(Scene &&scene) {
		m_NewScene = std::move(scene);
	}

	Scene *SceneManager::Impl::GetActive() {
		if (!m_Scene.has_value())
			return nullptr;

		return &m_Scene.value();
	}

	void SceneManager::Impl::PreUpdate() {
		if (!m_Scene.has_value())
			return;

		m_Scene->PreUpdate();
	}

	void SceneManager::Impl::Update() {
		if (!m_Scene.has_value())
			return;

		m_Scene->Update();
	}

	void SceneManager::Impl::PostUpdate() {
		if (m_Scene.has_value())
			m_Scene->PostUpdate();

		if (m_NewScene.has_value()) {
			m_Scene    = std::move(m_NewScene);
			m_NewScene = std::nullopt;
		}
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

	Scene *SceneManager::GetActive() {
		return m_pImpl->GetActive();
	}

	void SceneManager::PreUpdate() {
		m_pImpl->PreUpdate();
	}

	void SceneManager::Update() {
		m_pImpl->Update();
	}

	void SceneManager::PostUpdate() {
		m_pImpl->PostUpdate();
	}

	void SceneManager::FixedUpdate() {
		m_pImpl->FixedUpdate();
	}

	void SceneManager::Render() const {
		m_pImpl->Render();
	}
}// namespace roingine
