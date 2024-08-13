#include "upgrade.h"

#include <roingine/components/rect_collider.h>
#include <roingine/components/texture_renderer.h>

namespace bomberman {
	constexpr float c_Size{90.f};

	Upgrade::Upgrade(roingine::GameObject gameObject, UpgradeType upgrade)
	    : Component{gameObject}
	    , m_Upgrade{upgrade} {
		std::string const texturePath = upgrade_type::UpgradeTypeTexturePath(upgrade);
		gameObject.AddComponent<roingine::TextureRenderer>(
		        texturePath, c_Size, c_Size, roingine::ScalingMethod::NearestNeighbor
		);
		gameObject.AddComponent<roingine::RectCollider>(c_Size, c_Size);
	}

	UpgradeType Upgrade::GetUpgrade() const noexcept {
		return m_Upgrade;
	}
}// namespace bomberman
