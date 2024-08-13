#ifndef MUSIC_IMPL_H
#define MUSIC_IMPL_H

#include "roingine/music.h"

#include <SDL_mixer.h>

namespace roingine {
	class MixMusicDestroyer final {
	public:
		void operator()(Mix_Music *ptr) const;
	};

	class Music::Impl final {
		std::unique_ptr<Mix_Music, MixMusicDestroyer> m_pMusic;

	public:
		explicit Impl(std::string_view path);

		void Play() const;

		static void Stop();
	};
}// namespace roingine

#endif//MUSIC_IMPL_H
