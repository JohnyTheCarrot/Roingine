#include <roingine/engine_event_queue.h>
#include <roingine/game_info.h>

namespace roingine {
	GameInfo::GameInfo()
	    : m_KeyHeldThresholdMs{300} {};

	GameInfo::~GameInfo() = default;

	float GameInfo::GetKeyHeldThresholdMs() const noexcept {
		return m_KeyHeldThresholdMs;
	}

	void GameInfo::SetKeyHeldThresholdMs(float threshold) noexcept {
		m_KeyHeldThresholdMs = threshold;
	}

	bool GameInfo::IsRunning() const noexcept {
		return m_IsRunning;
	}

	void GameInfo::QuitGame() {
		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::CleanShutdown>();
		m_IsRunning = false;
	}
}// namespace roingine
