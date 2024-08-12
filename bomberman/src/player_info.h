#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <optional>
#include <roingine/Singleton.h>

namespace bomberman {
	class PlayerInfo final {
		int  m_Score{0};
		bool m_IsPlayer1{true};

	public:
		PlayerInfo();
	};

	struct PlayerInfoContainer final : roingine::Singleton<PlayerInfoContainer> {
		PlayerInfo                m_Player1Info;
		std::optional<PlayerInfo> m_Player2Info{std::nullopt};
	};
}// namespace bomberman

#endif//PLAYER_INFO_H
