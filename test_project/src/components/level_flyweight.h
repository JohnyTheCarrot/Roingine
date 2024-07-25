#ifndef LEVEL_FLYWEIGHT_H
#define LEVEL_FLYWEIGHT_H

#include <roingine/components/component.h>
#include <roingine/reusable_texture.h>

namespace roingine {
	class Transform;
}

namespace bomberman {
	class LevelFlyweight final : public roingine::Component {
		enum class TileType { Nothing, SolidWall, BrickWall };

		roingine::Transform  *m_rpTransform;
		std::vector<TileType> m_TileGrid;

		// TODO: perhaps we should make a resource manager to handle textures because they're the same for all levels
		roingine::ReusableTexture m_SolidWallTexture;
		roingine::ReusableTexture m_BrickWallTexture;

	public:
		static constexpr int   c_LevelWidth{31};
		static constexpr int   c_LevelHeight{13};
		static constexpr float c_TileSize{90.f};

		explicit LevelFlyweight(roingine::GameObject &gameObject);

		void Render() const override;
	};
}// namespace bomberman

#endif//LEVEL_FLYWEIGHT_H
