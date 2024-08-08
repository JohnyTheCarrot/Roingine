#include "scene_manager_impl.h"

namespace roingine {
	SceneManager::SceneManager()
	    : m_pImpl{std::make_unique<Impl>()} {
	}

	SceneManager::~SceneManager() = default;

	void SceneManager::Impl::SetActive(Scene &&scene) {
		m_NewScene = std::move(scene);
	}

	void SceneManager::Impl::UnloadScene(bool immediate) {
		m_NewScene = SceneUnload{};

		if (immediate) {
			LoadNextImmediately();
		}
	}

	void SceneManager::Impl::LoadNextImmediately() {
		if (std::holds_alternative<Scene>(m_NewScene)) {
			auto &newScene{std::get<Scene>(m_NewScene)};

			m_Scene    = std::move(newScene);
			m_NewScene = NoNewScene{};
			return;
		}

		if (std::holds_alternative<SceneUnload>(m_NewScene)) {
			m_Scene.reset();
			m_NewScene = NoNewScene{};
		}
	}

	Scene *SceneManager::Impl::GetActive() {
		if (std::holds_alternative<Scene>(m_NewScene)) {
			return &std::get<Scene>(m_NewScene);
		}

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

		LoadNextImmediately();
	}

	void SceneManager::Impl::FixedUpdate() {
		if (!m_Scene.has_value())
			return;

		m_Scene->FixedUpdate();
	}

	void SceneManager::Impl::Render() const {
		if (!m_Scene.has_value())
			return;

		m_Scene->RenderFromCameras();
	}

	void SceneManager::SetActive(Scene &&scene) const {
		m_pImpl->SetActive(std::move(scene));
	}

	void SceneManager::UnloadScene(bool immediate) const {
		m_pImpl->UnloadScene(immediate);
	}

	Scene *SceneManager::GetActive() const {
		return m_pImpl->GetActive();
	}

	void SceneManager::PreUpdate() const {
		m_pImpl->PreUpdate();
	}

	void SceneManager::Update() const {
		m_pImpl->Update();
	}

	void SceneManager::PostUpdate() const {
		m_pImpl->PostUpdate();
	}

	void SceneManager::FixedUpdate() const {
		m_pImpl->FixedUpdate();
	}

	void SceneManager::RenderFromCameras() const {
		m_pImpl->Render();
	}
}// namespace roingine
