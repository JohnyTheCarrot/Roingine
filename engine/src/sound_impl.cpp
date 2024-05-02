#include "sound_impl.h"

namespace roingine {
	SoundClip::SoundClip(FileType fileType, std::string_view path)
	    : m_pImpl{std::make_unique<Impl>(fileType, std::move(path))} {};

	SoundClip::Impl::Impl(FileType fileType, std::string_view path)
	    : m_Sound{nullptr} {
		Mix_Chunk *ptr{};

		switch (fileType) {
			case FileType::WAV:
				auto const pathPtr{path.data()};
				std::cout << pathPtr << std::endl;
				ptr = Mix_LoadWAV(pathPtr);
				if (ptr == nullptr) {
					throw SoundFileLoadFailed{Mix_GetError()};
				}

				m_Sound.reset(ptr);
		}
	}

	void SoundClip::Impl::Play() {
		Mix_PlayChannel(-1, m_Sound.get(), 0);
	}

	SoundClip::~SoundClip() = default;

	void SoundClip::Play() const {
		m_pImpl->Play();
	}

	SoundClip::SoundClip(SoundClip &&) = default;

	SoundClip &SoundClip::operator=(SoundClip &&) = default;

	void MixChunkDestroyer::operator()(Mix_Chunk *ptr) {
		Mix_FreeChunk(ptr);
	}


}// namespace roingine
