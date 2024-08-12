#include "components/player.h"
#include "event_queue.h"
#include "game.h"

#include <roingine/audio_service.h>
#include <roingine/event_queue.h>
#include <roingine/game_info.h>
#include <roingine/keyboard_input.h>
#include <roingine/roingine.h>
#include <roingine/scene.h>
#include <roingine/service_locator.h>

using namespace roingine;

constexpr int WINDOW_WIDTH  = 960;
constexpr int WINDOW_HEIGHT = 870;

int main() {
	Engine roingine{"Bomberman", WINDOW_WIDTH, WINDOW_HEIGHT};
	roingine.SetClearColor(56.f / 255.f, 135.f / 255.f, 0.f);

	KeyboardInput::Provide(std::make_unique<SDLKeyboardInputService>());

	Scene scene{};

	bomberman::Game game{std::move(scene), WINDOW_WIDTH, WINDOW_HEIGHT};

	roingine.Run([]() { bomberman::event_queue::EventQueue::GetInstance().Update(); });
}
