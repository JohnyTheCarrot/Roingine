#ifndef UPGRADE_TYPE_H
#define UPGRADE_TYPE_H
#include <string>

namespace bomberman {
	enum class UpgradeType {
		ExtraBomb = 0,
		Flames    = 1,
		Detonator = 2,
	};

	namespace upgrade_type {
		[[nodiscard]]
		std::string UpgradeTypeTexturePath(UpgradeType upgradeType);
	}// namespace upgrade_type
}// namespace bomberman


#endif//UPGRADE_TYPE_H
