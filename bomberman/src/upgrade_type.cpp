#include "upgrade_type.h"

#include <cassert>

namespace bomberman::upgrade_type {
	std::string UpgradeTypeTexturePath(UpgradeType upgradeType) {
		switch (upgradeType) {
			case UpgradeType::ExtraBomb:
				return "res/img/upgrade_extra_bomb.png";
			case UpgradeType::Flames:
				return "res/img/upgrade_flames.png";
			case UpgradeType::Detonator:
				return "res/img/upgrade_detonator.png";
		}

		assert(false);
		return "";
	}
}// namespace bomberman::upgrade_type
