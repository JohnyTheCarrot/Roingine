#include <roingine/components/rect_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/roingine.h>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>

using namespace roingine;

int main() {
	roingine::Engine::Settings roingineSettings{.gameTitle = "Bubble Bobble", .windowWidth = 640, .windowHeight = 480};
	roingine::Engine           roingine{roingineSettings};

	Scene      scene{};
	GameObject go{scene.AddGameObject()};
	go.AddComponent<Transform>(math::Vec2{100.0, 10.0}, 24.0);
	go.AddComponent<RectRenderer>(100, 100);

	SceneManager::GetInstance().SetActive(std::move(scene));
	roingine.Run();
}
