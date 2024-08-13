#ifndef MUSIC_H
#define MUSIC_H

#include <exception>
#include <format>
#include <memory>

namespace roingine {
	class MusicFileLoadFailed final : public std::exception {
	public:
		explicit MusicFileLoadFailed(char const *mixError)
		    : m_ErrorMessage{std::format("Failed to load sound file: {}", mixError)} {};

		[[nodiscard]]
		char const *what() const noexcept override {
			return m_ErrorMessage.c_str();
		}

	private:
		std::string m_ErrorMessage;
	};

	class Music final {
		class Impl;
		std::unique_ptr<Impl> m_pImpl;

	public:
		explicit Music(std::string_view path);

		~Music();

		Music(Music &&) noexcept;

		Music &operator=(Music &&) noexcept;

		Music(Music const &) = delete;

		Music &operator=(Music const &) = delete;

		void Play() const;

		static void Stop();
	};
}// namespace roingine

#endif//MUSIC_H
