#include "components/player.h"
#include "game.h"

#include <roingine/audio_service.h>
#include <roingine/engine_event_queue.h>
#include <roingine/event_queue.h>
#include <roingine/game_info.h>
#include <roingine/keyboard_input.h>
#include <roingine/roingine.h>
#include <roingine/scene.h>
#include <roingine/service_locator.h>
#include <unordered_map>

using namespace roingine;

enum class Sounds { TestSound };

enum class EventType { PlaySoundRequest };

template<>
struct roingine::EventTypeData<EventType, EventType::PlaySoundRequest> final {
	struct Data_t final {
		Sounds sound;
	};
};// namespace roingine

using PlaySoundRequestData = EventTypeData<EventType, EventType::PlaySoundRequest>::Data_t;

using GameEventQueue = EventQueue<EventType, PlaySoundRequestData>;


using Audio               = AudioSystem<Sounds>;
using AudioServiceLocator = ServiceLocator<AudioService<Sounds>>;

constexpr int WINDOW_WIDTH  = 960;
constexpr int WINDOW_HEIGHT = 870;

int main() {
	Engine roingine{"Test", WINDOW_WIDTH, WINDOW_HEIGHT};
	roingine.SetClearColor(56.f / 255.f, 135.f / 255.f, 0.f);

	std::unordered_map<Sounds, std::string> soundMap{};
	soundMap.emplace(Sounds::TestSound, "sound.wav");

	AudioServiceLocator::Provide(std::make_unique<Audio>(std::move(soundMap)));

	KeyboardInput::Provide(std::make_unique<SDLKeyboardInputService>());

	Scene scene{};

	bomberman::Game game{std::move(scene), WINDOW_WIDTH, WINDOW_HEIGHT};

	roingine.Run([]() { GameEventQueue::GetInstance().Update(); });
}
