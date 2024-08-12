#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <optional>
#include <roingine/Singleton.h>
#include <string>

namespace bomberman {
	class PlayerInfo final {
		std::optional<std::string> m_ControllerSerial{std::nullopt};
		int                        m_Score{0};
		bool                       m_ControllerConnected{false};
		bool                       m_IsPlayer1{true};

	public:
		PlayerInfo();

		void SetControllerSerial(std::string controllerSerial) noexcept;

		[[nodiscard]]
		std::optional<std::string> const &GetControllerSerial() const noexcept;

		void SetControllerConnected(bool controllerConnected) noexcept;

		[[nodiscard]]
		bool IsControllerConnected() const noexcept;
	};

	struct PlayerInfoContainer final : roingine::Singleton<PlayerInfoContainer> {
		PlayerInfo                m_Player1Info;
		std::optional<PlayerInfo> m_Player2Info{std::nullopt};
	};
}// namespace bomberman

#endif//PLAYER_INFO_H
