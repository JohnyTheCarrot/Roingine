#include <SDL_mixer.h>
#include <roingine/engine_event_queue.h>
#include <roingine/game_info.h>

namespace roingine {
	GameInfo::GameInfo()
	    : m_KeyHeldThresholdMs{0.3f} {};

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

	void GameInfo::ToggleMute() {
		Mix_MasterVolume(m_IsMuted ? MIX_MAX_VOLUME : 0);
		Mix_VolumeMusic(m_IsMuted ? MIX_MAX_VOLUME : 0);
		m_IsMuted = !m_IsMuted;
	}
}// namespace roingine
