#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <roingine/input.h>
#include <roingine/singleton.h>

namespace roingine {
	class GameInfo final : public Singleton<GameInfo> {
	public:
		GameInfo();
		~GameInfo();

		[[nodiscard]]
		float GetKeyHeldThresholdMs() const noexcept;

		void SetKeyHeldThresholdMs(float threshold) noexcept;

		[[nodiscard]]
		bool IsRunning() const noexcept;

		void QuitGame();

	private:
		float m_KeyHeldThresholdMs;
		bool  m_IsRunning{true};
	};
}// namespace roingine

#endif// GAME_INFO_H
