#ifndef AUDIO_SERVICE_H
#define AUDIO_SERVICE_H

#include <queue>
#include <roingine/sound_clip.h>
#include <thread>
#include <unordered_map>

namespace roingine {
	class SoundClipNoPath final : public std::exception {
	public:
		char const *what() const noexcept override {
			return "The requested sound has no path defined";
		}
	};

	template<typename SoundEnum>
	    requires std::is_enum_v<SoundEnum>
	class AudioService {
	public:
		virtual ~AudioService(){};

		virtual void Play(SoundEnum sound) = 0;
	};

	template<typename SoundEnum>
	class AudioSystem final : public AudioService<SoundEnum> {
	public:
		explicit AudioSystem(std::unordered_map<SoundEnum, std::string> &&map)
		    : m_SoundClipPaths{std::move(map)} {
			std::jthread thread{&AudioSystem::RunThread, this};
			thread.join();
		}

		virtual ~AudioSystem() {
		}

		void Play(SoundEnum sound) override {
			if (!m_SoundClipPaths.contains(sound))
				throw SoundClipNoPath{};

			m_SoundClipQueue.emplace(sound);
		}

	private:
		void RunThread() {
		}

		std::queue<SoundEnum>                      m_SoundClipQueue;
		std::unordered_map<SoundEnum, std::string> m_SoundClipPaths;
	};

	template<typename SoundEnum>
	class NullAudioSystem final : public AudioService<SoundEnum> {
		void Play(SoundEnum) override {
		}
	};
}// namespace roingine

#endif// AUDIO_SERVICE_H
