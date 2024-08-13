#ifndef UPGRADE_TYPE_H
#define UPGRADE_TYPE_H
#include <string>

namespace bomberman {
	enum class UpgradeType {
		ExtraBomb,
		Flames,
		Detonator,
	};

	namespace upgrade_type {
		[[nodiscard]]
		std::string UpgradeTypeTexturePath(UpgradeType upgradeType);
	}// namespace upgrade_type
}// namespace bomberman


#endif//UPGRADE_TYPE_H
