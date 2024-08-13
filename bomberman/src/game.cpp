#include "game.h"

#include "audio.h"
#include "components/player.h"
#include "level.h"
#include "player_info.h"

#include <fstream>
#include <roingine/audio_service.h>
#include <roingine/service_locator.h>

namespace bomberman {
	void Game::OnControllerConnected(roingine::event_queue::ControllerConnectedData const &data) {
		if (!m_UnassignedController1.has_value()) {
			m_UnassignedController1.emplace(data.rpController, m_Level, [this]() { m_UnassignedController1.reset(); });

			return;
		}

		if (!m_UnassignedController2.has_value()) {
			m_UnassignedController2.emplace(data.rpController, m_Level, [this]() { m_UnassignedController2.reset(); });
		}
	}

	void Game::OnControllerDisconnected(roingine::event_queue::ControllerDisconnectedData const &data) {
		if (m_Level.DoesPlayer1OwnController(*data.rpController)) {
			m_Level.SetPlayer1Controller(nullptr);
			return;
		}

		if (m_Level.DoesPlayer2OwnController(*data.rpController)) {
			m_Level.SetPlayer2Controller(nullptr);
		}
	}

	Game::Game(int windowWidth, int windowHeight)
	    : m_hControllerConnected{roingine::event_queue::EventQueue::GetInstance()
	                                     .AttachEventHandler<roingine::event_queue::EventType::ControllerConnected>(
	                                             [this](auto const &data) { OnControllerConnected(data); }
	                                     )}
	    , m_hControllerDisconnected{roingine::event_queue::EventQueue::GetInstance()
	                                        .AttachEventHandler<
	                                                roingine::event_queue::EventType::ControllerDisconnected>(
	                                                [this](auto const &data) { OnControllerDisconnected(data); }
	                                        )}
	    , m_LevelSetupData{[this] {
		    std::ifstream file{"res/level_setup_data.bin", std::ios::in | std::ios::binary};
		    if (!file) {
			    throw std::runtime_error{"Failed to open level setup data file"};
		    }

		    size_t numLevels{};
		    file.seekg(0, std::ios::end);
		    numLevels = file.tellg() / sizeof(LevelSetupData);
		    file.seekg(0, std::ios::beg);

		    std::vector<LevelSetupData> levelSetupData(numLevels);
		    for (size_t i{}; i < numLevels; ++i) {
			    file.read(reinterpret_cast<char *>(&levelSetupData[i]), sizeof(LevelSetupData));
		    }

		    std::cout << "Loaded " << numLevels << " levels" << std::endl;
		    return levelSetupData;
	    }()}
	    , m_Level{LevelLoadInfo{
	              .windowWidth  = windowWidth,
	              .windowHeight = windowHeight,
	              .setupData    = m_LevelSetupData.at(0)
	      }}
	    , m_WindowWidth{windowWidth}
	    , m_WindowHeight{windowHeight} {
		using FileType = roingine::SoundClip::FileType;

		std::unordered_map<audio::Sound, roingine::SoundClip> soundMap{};
		soundMap.emplace(audio::Sound::BombExplode, roingine::SoundClip{FileType::WAV, "res/sound/explosion.wav"});
		soundMap.emplace(audio::Sound::BombPlace, roingine::SoundClip{FileType::WAV, "res/sound/bomb_place.wav"});
		soundMap.emplace(audio::Sound::WalkVertical, roingine::SoundClip{FileType::WAV, "res/sound/walk_vertical.wav"});
		soundMap.emplace(
		        audio::Sound::WalkHorizontal, roingine::SoundClip{FileType::WAV, "res/sound/walk_horizontal.wav"}
		);
		soundMap.emplace(audio::Sound::PlayerDeath, roingine::SoundClip{FileType::WAV, "res/sound/player_death.wav"});

		audio::AudioServiceLocator::Provide(std::make_unique<roingine::AudioSystem<audio::Sound>>(std::move(soundMap)));
	}

	void Game::LoadNextLevel() {
		if (m_CurrentLevelIndex + 1 >= m_LevelSetupData.size()) {
			return;
		}

		++m_CurrentLevelIndex;

		m_Level = Level{LevelLoadInfo{
		        .windowWidth  = m_WindowWidth,
		        .windowHeight = m_WindowHeight,
		        .setupData    = m_LevelSetupData.at(m_CurrentLevelIndex)
		}};
	}
}// namespace bomberman
