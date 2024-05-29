#include <roingine/game_time.h>
#include <thread>

using namespace std::literals::chrono_literals;

namespace roingine {
	void GameTime::StartDeltaTimeMeasurement() {
		m_Start = std::chrono::high_resolution_clock::now();
	}

	void GameTime::EndDeltaTimeMeasurement() {
		auto const now{std::chrono::high_resolution_clock::now()};
		m_DeltaTime = std::chrono::duration_cast<GameLoopTimeUnit>(now - m_Start);
		m_FpsTimeProgress += m_DeltaTime;
		++m_Frames;

		if (m_FpsTimeProgress >= 1s) {
			m_Fps             = m_Frames * static_cast<float>(DURATION_FRACTION) / m_FpsTimeProgress.count();
			m_FpsTimeProgress = 0s;
			m_Frames          = 0;
			// std::cout << m_Fps << " FPS" << std::endl;
		}
	}

	void GameTime::Sleep() {
		auto const sleep_time{m_Start + TIME_PER_FRAME - std::chrono::high_resolution_clock::now()};
		std::this_thread::sleep_for(sleep_time);
	}

	GameTime::DurationPrecision GameTime::GetDeltaTime() const noexcept {
		return m_DeltaTime.count() / DURATION_FRACTION;
	}

	GameTime::DurationPrecision GameTime::GetFps() const noexcept {
		return m_Fps;
	}

	GameTime::GameTime() = default;

	GameTime::~GameTime() = default;
}// namespace roingine
