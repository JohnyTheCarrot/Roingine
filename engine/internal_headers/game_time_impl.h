#ifndef GAME_TIME_INTERNAL_H
#define GAME_TIME_INTERNAL_H

#include <chrono>
#include <roingine/game_time.h>

namespace roingine {
	constexpr size_t DURATION_FRACTION{1000};
	using banana           = std::chrono::seconds;
	using GameLoopTimeUnit = std::chrono::duration<GameTime::DurationPrecision, std::ratio<1, DURATION_FRACTION>>;

	constexpr GameLoopTimeUnit operator""_t(unsigned long long time) {
		return GameLoopTimeUnit{time};
	}

	class GameTime::Impl final {
	public:
		static constexpr GameTime::DurationPrecision FIXED_TIME_DELTA{(10_t).count()};
		static constexpr std::chrono::duration       TIME_PER_FRAME{
                std::chrono::duration<GameTime::DurationPrecision, std::ratio<1>>{1} / 144.0
        };

		[[nodiscard]]
		GameTime::DurationPrecision GetDeltaTime() const noexcept {
			return m_DeltaTime.count();
		}

		[[nodiscard]]
		GameTime::DurationPrecision GetFps() const noexcept {
			return m_Fps;
		}

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

#endif// GAME_TIME_INTERNAL_H
