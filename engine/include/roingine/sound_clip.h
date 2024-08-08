#ifndef SOUND_H
#define SOUND_H

#include <exception>
#include <format>
#include <memory>

class SoundFileLoadFailed final : public std::exception {
public:
	explicit SoundFileLoadFailed(char const *mixError)
	    : m_ErrorMessage{std::format("Failed to load sound file: {}", mixError)} {};

	[[nodiscard]]
	char const *what() const noexcept override {
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

		SoundClip(SoundClip &&) noexcept;

		SoundClip &operator=(SoundClip &&) noexcept;

		void Play() const;

	private:
		class Impl;

		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// SOUND_H
