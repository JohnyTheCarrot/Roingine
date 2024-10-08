#ifndef SOUND_IMPL_H
#define SOUND_IMPL_H

#include <SDL_mixer.h>
#include <roingine/sound_clip.h>

namespace roingine {
	class MixChunkDestroyer final {
	public:
		void operator()(Mix_Chunk *ptr) const;
	};

	class SoundClip::Impl final {
	public:
		Impl(FileType fileType, std::string_view path);

		void Play() const;

	private:
		std::unique_ptr<Mix_Chunk, MixChunkDestroyer> m_Sound;
	};
}// namespace roingine

#endif// SOUND_IMPL_H
