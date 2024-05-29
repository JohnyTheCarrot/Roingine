#ifndef GAME_TIME_H
#define GAME_TIME_H

#include <chrono>
#include <roingine/singleton.h>

namespace roingine {
	class GameTime final : public Singleton<GameTime> {
	public:
		static constexpr size_t DURATION_FRACTION{1000};
		using DurationPrecision = float;
		using GameLoopTimeUnit  = std::chrono::duration<DurationPrecision, std::ratio<1, DURATION_FRACTION>>;

		static constexpr DurationPrecision     FIXED_TIME_DELTA{0.01f};
		static constexpr std::chrono::duration TIME_PER_FRAME{
		        std::chrono::duration<DurationPrecision, std::ratio<1>>{1} / 144.0
		};

		GameTime();
		~GameTime();

		[[nodiscard]]
		DurationPrecision GetDeltaTime() const noexcept;

		[[nodiscard]]
		DurationPrecision GetFps() const noexcept;

		void StartDeltaTimeMeasurement();

		void EndDeltaTimeMeasurement();

		void Sleep();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start{};

		GameLoopTimeUnit m_DeltaTime{};
		GameLoopTimeUnit m_FpsTimeProgress{};
		float            m_Fps{};
		int              m_Frames{};
	};
}// namespace roingine

#endif// GAME_TIME_H
