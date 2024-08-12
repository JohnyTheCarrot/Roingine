#include "player_info.h"

namespace bomberman {
	PlayerInfo::PlayerInfo() = default;

	void PlayerInfo::SetControllerSerial(std::string controllerSerial) noexcept {
		m_ControllerSerial = std::move(controllerSerial);
	}

	std::optional<std::string> const &PlayerInfo::GetControllerSerial() const noexcept {
		return m_ControllerSerial;
	}

	void PlayerInfo::SetControllerConnected(bool controllerConnected) noexcept {
		m_ControllerConnected = controllerConnected;
	}

	bool PlayerInfo::IsControllerConnected() const noexcept {
		return m_ControllerConnected;
	}
}// namespace bomberman
