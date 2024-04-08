#include <iostream>
#include <roingus.h>

using namespace roingus;

int main() {
	roingus::Engine::Settings roingineSettings{.gameTitle = "Bubble Bobble", .windowWidth = 640, .windowHeight = 480};
	roingus::Engine           roingine{roingineSettings};
}
