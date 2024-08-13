#include "player_info.h"

namespace bomberman {
	void PlayerInfo::UnlockUpgrade(UpgradeType upgrade) {
		switch (upgrade) {
			case UpgradeType::Flames:
				++m_BombRange;
				break;
			default:
				// TODO
				break;
		}
	}
}// namespace bomberman
