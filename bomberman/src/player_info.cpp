#include "player_info.h"

namespace bomberman {
	void PlayerInfo::UnlockUpgrade(UpgradeType upgrade) {
		switch (upgrade) {
			case UpgradeType::Flames:
				++m_BombRange;
				break;
			case UpgradeType::ExtraBomb:
				++m_MaxBombs;
				break;
			case UpgradeType::Detonator:
				m_HasDetonator = true;
				break;
			default:
				break;
		}
	}
}// namespace bomberman
