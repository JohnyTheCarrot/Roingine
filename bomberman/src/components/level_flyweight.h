#ifndef LEVEL_FLYWEIGHT_H
#define LEVEL_FLYWEIGHT_H

#include "../event_queue.h"

#include <roingine/components/component.h>
#include <roingine/event_queue.h>
#include <roingine/reusable_texture.h>

namespace roingine {
	class RectCollider;
	class Transform;
}// namespace roingine

namespace bomberman {
	class LevelFlyweight final : public roingine::Component {
	public:
		enum class TileType { Nothing, SolidWall, BrickWall };

		static constexpr int   c_LevelWidth{31};
		static constexpr int   c_LevelHeight{13};
		static constexpr float c_TileSize{90.f};

		explicit LevelFlyweight(roingine::GameObject &gameObject);

		void Render() const override;

		[[nodiscard]]
		bool IsCollidingWith(roingine::RectCollider const &collider) const;

		[[nodiscard]]
		std::optional<glm::vec2> GetCollisionPoint(glm::vec2 origin, float width, float height) const;

		[[nodiscard]]
		TileType GetTileType(int gridX, int gridY) const;

		[[nodiscard]]
		glm::vec2 GridToPosition(int x, int y) const;

		[[nodiscard]]
		glm::vec2 SnapToGrid(glm::vec2 position) const;

		[[nodiscard]]
		std::pair<int, int> PositionToGrid(glm::vec2 position) const;

		[[nodiscard]]
		glm::vec2 GetRandomEmptyTilePos() const;

	private:
		roingine::EventHandlerHandle<event_queue::EventQueue> m_hBombPlaceRequestHandler;
		roingine::EventHandlerHandle<event_queue::EventQueue> m_hBombDetonatedHandler;

		std::vector<TileType> m_TileGrid;
		// TODO: perhaps we should make a resource manager to handle textures because they're the same for all levels
		roingine::ReusableTexture m_SolidWallTexture;
		roingine::ReusableTexture m_BrickWallTexture;

		roingine::Transform *m_rpTransform;

		[[nodiscard]]
		bool IsPointInWall(glm::vec2 const &point) const;

		void BombDetonatedHandler(event_queue::BombDetonatedData const &data);

		void BombPlaceRequestHandler(event_queue::BombPlaceRequestData const &data) const;

		int ExplodeTiles(
		        glm::vec2 startPos, bool isX, int range, std::function<bool(int, int)> const &comp, int xIndex,
		        int yIndex
		);
	};
}// namespace bomberman

#endif//LEVEL_FLYWEIGHT_H
