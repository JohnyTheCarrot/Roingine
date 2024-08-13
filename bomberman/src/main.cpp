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
	Engine const roingine{"Bomberman", WINDOW_WIDTH, WINDOW_HEIGHT};
	roingine.SetClearColor(56.f / 255.f, 135.f / 255.f, 0.f);

	std::cout << "Hello, Tom/Alex. Please be sure to read the following:\n";
	std::cout << "WASD to move, E to place bomb, F to detonate bomb (if unlocked), M to mute/unmute\n, F1 to skip "
	             "level\n";
	std::cout
	        << "As for the controller, the D-Pad is used to move, X to place bomb, Y to detonate bomb (if unlocked)\n";
	std::cout << "\"But Tuur, I specifically asked for co-op and versus modes!\"\n";
	std::cout << "Indeed, you did! Good thing you read this, because to enter co-op mode, you press the RIGHT SHOULDER "
	             "button on the player 2 controller!\n";
	std::cout << "To enter versus mode, you press the RIGHT STICK on the player 2 controller!\n";
	std::cout << "\"I also required player 1 to be playable with the controller!\"\n";
	std::cout << "Yes, yes. Player 1's controller is enabled by tapping the LEFT SHOULDER button on the player 1 "
	             "controller.\n";
	std::cout << "Because I don't want you to get damaged while reading this, I'll require you to press any key to "
	             "continue.\n";
	std::cout << std::endl;

	std::cin.get();

	KeyboardInput::Provide(std::make_unique<SDLKeyboardInputService>());

	bomberman::Game game{WINDOW_WIDTH, WINDOW_HEIGHT};

	roingine.Run([&game]() {
		bomberman::event_queue::EventQueue::GetInstance().Update();

		const auto &keyboardInput{KeyboardInput::GetService()};

		if (keyboardInput.GetKeyState(InputKeys::M) == KeyEventType::Down) {
			GameInfo::GetInstance().ToggleMute();
		}

		if (keyboardInput.GetKeyState(InputKeys::F1) == KeyEventType::Down) {
			game.LoadNextLevel();
		}
	});
}
