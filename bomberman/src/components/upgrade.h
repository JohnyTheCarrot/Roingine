#ifndef UPGRADE_H
#define UPGRADE_H

#include "../upgrade_type.h"

#include <roingine/components/component.h>

namespace bomberman {
	class Upgrade final : public roingine::Component {
		UpgradeType m_Upgrade;

	public:
		Upgrade(roingine::GameObject gameObject, UpgradeType upgrade);

		[[nodiscard]]
		UpgradeType GetUpgrade() const noexcept;
	};
}// namespace bomberman

#endif//UPGRADE_H
