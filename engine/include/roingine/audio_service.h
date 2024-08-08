#ifndef AUDIO_SERVICE_H
#define AUDIO_SERVICE_H

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <roingine/sound_clip.h>
#include <thread>
#include <unordered_map>

namespace roingine {
	class SoundClipNoPath final : public std::exception {
	public:
		[[nodiscard]]
		char const *what() const noexcept override {
			return "The requested sound has no path defined";
		}
	};

	template<typename SoundEnum>
	    requires std::is_enum_v<SoundEnum>
	class AudioService {
	public:
		virtual ~AudioService() = default;

		virtual void Play(SoundEnum sound) = 0;
	};

	template<typename SoundEnum>
	class AudioSystem final : public AudioService<SoundEnum> {
	public:
		explicit AudioSystem(std::unordered_map<SoundEnum, SoundClip> &&map)
		    : m_Thread{std::bind(&AudioSystem::RunThread, this, std::placeholders::_1), this}
		    , m_SoundClipPaths{std::move(map)} {
		}

		~AudioSystem() override = default;

		void Play(SoundEnum sound) override {
			if (!m_SoundClipPaths.contains(sound))
				throw SoundClipNoPath{};

			m_SoundClipEndIndex                   = (m_SoundClipEndIndex + 1) % MAX_SOUND_CLIPS;
			m_SoundClipQueue[m_SoundClipEndIndex] = sound;
			m_ConditionVariable.notify_one();
		}

	private:
		static constexpr int MAX_SOUND_CLIPS{10};

		void RunThread(std::stop_token const &stopToken) {
			{
				std::lock_guard guard{m_Mutex};
				std::cout << "Audio thread started" << std::endl;
			}

			std::stop_callback callback{stopToken, [this] {
				                            std::lock_guard guard{m_Mutex};
				                            m_ConditionVariable.notify_one();
			                            }};

			while (!stopToken.stop_requested()) {
				std::unique_lock guard{m_Mutex};
				m_ConditionVariable.wait(guard, [this, &stopToken] {
					return stopToken.stop_requested() || m_SoundClipStartIndex != m_SoundClipEndIndex;
				});

				while (m_SoundClipStartIndex != m_SoundClipEndIndex) {
					m_SoundClipStartIndex = (m_SoundClipStartIndex + 1) % MAX_SOUND_CLIPS;
					m_SoundClipPaths.at(m_SoundClipQueue[m_SoundClipStartIndex]).Play();
				}
			}

			std::cout << "Audio thread stopped" << std::endl;
		}

		std::jthread            m_Thread;
		std::mutex              m_Mutex{};
		std::condition_variable m_ConditionVariable{};


		int m_SoundClipStartIndex{};
		int m_SoundClipEndIndex{};

		std::array<SoundEnum, MAX_SOUND_CLIPS>   m_SoundClipQueue{};
		std::unordered_map<SoundEnum, SoundClip> m_SoundClipPaths;
	};

	template<typename SoundEnum>
	class NullAudioSystem final : public AudioService<SoundEnum> {
		void Play(SoundEnum) override {
		}
	};
}// namespace roingine

#endif// AUDIO_SERVICE_H
