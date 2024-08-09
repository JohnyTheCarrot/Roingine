#include <roingine/components/component.h>

namespace roingine {
	Component::Component(GameObject gameObject)
	    : m_GameObject{gameObject} {
	}

	Component::Component(Component &&other) noexcept = default;

	Component::~Component() = default;

	GameObject &Component::GetGameObject() {
		return m_GameObject;
	}

	GameObject const &Component::GetGameObject() const {
		return m_GameObject;
	}

	bool Component::GetIsEnabled() const noexcept {
		return m_IsEnabled;
	}

	void Component::SetEnabled(bool isEnabled) noexcept {
		m_IsEnabled = isEnabled;
	}
}// namespace roingine
