#include "music_impl.h"
#include "roingine/sound_clip.h"

namespace roingine {
	void MixMusicDestroyer::operator()(Mix_Music *ptr) const {
		Mix_FreeMusic(ptr);
	}

	Music::Impl::Impl(std::string_view path)
	    : m_pMusic{nullptr} {
		auto const pathPtr{path.data()};
		Mix_Music *ptr{Mix_LoadMUS(pathPtr)};
		if (ptr == nullptr) {
			throw SoundFileLoadFailed{Mix_GetError()};
		}

		m_pMusic.reset(ptr);
	}

	void Music::Impl::Play() const {
		Mix_PlayMusic(m_pMusic.get(), -1);
	}

	void Music::Impl::Stop() {
		Mix_HaltMusic();
	}

	Music::Music(std::string_view path)
	    : m_pImpl{std::make_unique<Impl>(path)} {
	}

	Music::~Music() = default;

	Music::Music(Music &&) noexcept = default;

	Music &Music::operator=(Music &&) noexcept = default;

	void Music::Play() const {
		m_pImpl->Play();
	}

	void Music::Stop() {
		Impl::Stop();
	}
}// namespace roingine
