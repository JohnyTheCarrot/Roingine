#ifndef GAME_INFO_H
#define GAME_INFO_H

#include <roingine/singleton.h>

namespace roingine {
	class GameInfo final : public Singleton<GameInfo> {
	public:
		 GameInfo();
		~GameInfo() override;

		// TODO: these are trivial.
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
