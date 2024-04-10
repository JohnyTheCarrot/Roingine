#include <roingine.h>

using namespace roingine;

int main() {
	roingine::Engine::Settings roingineSettings{.gameTitle = "Bubble Bobble", .windowWidth = 640, .windowHeight = 480};
	roingine::Engine           roingine{roingineSettings};

	roingine.Run();
}
