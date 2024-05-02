#ifndef SOUND_H
#define SOUND_H

#include <exception>
#include <format>
#include <iostream>
#include <string_view>

class SoundFileLoadFailed : public std::exception {
public:
	SoundFileLoadFailed(char const *mixError)
	    : m_ErrorMessage{std::format("Failed to load sound file{}", mixError)} {
		std::cout << mixError << std::endl;
	};

	[[nodiscard]]
	char const *what() const noexcept {
		return m_ErrorMessage.c_str();
	}

private:
	std::string m_ErrorMessage;
};

namespace roingine {
	class SoundClip final {
	public:
		enum class FileType { WAV };

		SoundClip(FileType fileType, std::string_view path);
		~SoundClip();

		SoundClip(SoundClip &&);

		SoundClip &operator=(SoundClip &&);

		void Play() const;

	private:
		class Impl;

		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// SOUND_H
