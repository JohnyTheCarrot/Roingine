#ifndef LEVEL_FLYWEIGHT_H
#define LEVEL_FLYWEIGHT_H

#include "../event_queue.h"

#include <roingine/event_queue.h>
#include <roingine/components/component.h>
#include <roingine/reusable_texture.h>

namespace roingine {
	class RectCollider;
	class Transform;
}// namespace roingine

namespace bomberman {
	class LevelFlyweight final : public roingine::Component {
		enum class TileType { Nothing, SolidWall, BrickWall };

		roingine::EventHandlerHandle<event_queue::EventQueue> m_hBombDetonatedHandler;

		std::vector<TileType> m_TileGrid;
		// TODO: perhaps we should make a resource manager to handle textures because they're the same for all levels
		roingine::ReusableTexture m_SolidWallTexture;
		roingine::ReusableTexture m_BrickWallTexture;

		roingine::Transform *m_rpTransform;

		[[nodiscard]]
		bool IsPointInWall(glm::vec2 const &point) const;

		void BombDetonatedHandler(event_queue::BombDetonatedData const &data);

	public:
		static constexpr int   c_LevelWidth{31};
		static constexpr int   c_LevelHeight{13};
		static constexpr float c_TileSize{90.f};

		explicit LevelFlyweight(roingine::GameObject &gameObject);

		void Render() const override;

		[[nodiscard]]
		bool IsCollidingWith(roingine::RectCollider const &collider) const;

		[[nodiscard]]
		std::optional<glm::vec2> GetCollisionPoint(glm::vec2 origin, float width, float height) const;
	};
}// namespace bomberman

#endif//LEVEL_FLYWEIGHT_H
