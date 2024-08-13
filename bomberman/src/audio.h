#ifndef AUDIO_H
#define AUDIO_H

#include <roingine/audio_service.h>
#include <roingine/service_locator.h>

namespace bomberman::audio {
	enum class Sound {
		BombExplode,
		BombPlace,
		WalkVertical,
		WalkHorizontal,
		PlayerDeath,
	};

	using AudioServiceLocator = roingine::ServiceLocator<roingine::AudioService<Sound>>;
}// namespace bomberman::audio

#endif//AUDIO_H
