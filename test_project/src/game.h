#ifndef GAME_H
#define GAME_H

#include <roingine/engine_event_queue.h>
#include <roingine/event_queue.h>
#include <roingine/gameobject.h>
#include <roingine/scene.h>

namespace roingine {
	class Camera;

	class Transform;
}// namespace roingine

namespace bomberman {
	class Player;

	struct PlayerAndCamera final {
		Player           *m_rpPlayer{};
		roingine::Camera *m_rpCamera{};

		PlayerAndCamera(
		        roingine::Scene &scene, bool hasKeyboardSupport, int viewX, int viewY, int viewWidth, int viewHeight
		);

		~PlayerAndCamera();

		PlayerAndCamera(PlayerAndCamera const &) = delete;

		PlayerAndCamera(PlayerAndCamera &&) noexcept;

		PlayerAndCamera &operator=(PlayerAndCamera const &) = delete;

		PlayerAndCamera &operator=(PlayerAndCamera &&) noexcept;

		[[nodiscard]]
		roingine::Camera &GetCamera() const;

		[[nodiscard]]
		Player &GetPlayer() const;
	};

	class Game final {
		roingine::Scene               *m_rpScene{};
		PlayerAndCamera                m_PlayerOne;
		std::optional<PlayerAndCamera> m_PlayerTwo;

		roingine::EventHandlerHandle<roingine::event_queue::EventQueue> m_ControllerConnectedHandler;
		roingine::EventHandlerHandle<roingine::event_queue::EventQueue> m_ControllerDisconnectedHandler;

		int m_WindowWidth;
		int m_WindowHeight;

		void ControllerConnectedHandler(roingine::event_queue::ControllerConnectedData const &data);

		void ControllerDisconnectedHandler(roingine::event_queue::ControllerDisconnectedData const &data);

	public:
		Game(roingine::Scene &&scene, int windowWidth, int windowHeight);
	};
}// namespace bomberman

#endif//GAME_H
