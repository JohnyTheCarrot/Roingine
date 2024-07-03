#include <roingine/audio_service.h>
#include <roingine/components/camera.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/engine_event_queue.h>
#include <roingine/event_queue.h>
#include <roingine/game_info.h>
#include <roingine/input.h>
#include <roingine/roingine.h>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>
#include <roingine/service_locator.h>
#include <unordered_map>

using namespace roingine;

enum class Sounds { TestSound };

enum class EventType { PlaySoundRequest };

template<>
struct EventTypeData<EventType, EventType::PlaySoundRequest> final {
	struct Data_t final {
		Sounds sound;
	};
};

using PlaySoundRequestData = typename EventTypeData<EventType, EventType::PlaySoundRequest>::Data_t;

using GameEventQueue = EventQueue<EventType, PlaySoundRequestData>;


using Audio               = AudioSystem<Sounds>;
using AudioServiceLocator = ServiceLocator<AudioService<Sounds>>;

constexpr int WINDOW_WIDTH  = 640;
constexpr int WINDOW_HEIGHT = 480;

int main() {
	roingine::Engine roingine{"Test", WINDOW_WIDTH, WINDOW_HEIGHT};

	std::unordered_map<Sounds, std::string> soundMap{};
	soundMap.emplace(Sounds::TestSound, "sound.wav");

	AudioServiceLocator::Provide(std::make_unique<Audio>(std::move(soundMap)));

	KeyboardInput::Provide(std::make_unique<SDLKeyboardInputService>());

	Scene scene{};
	auto  go1{scene.AddGameObject()};
	go1.AddComponent<Transform>(glm::vec2{50.f, 50.f}, 0.f);
	go1.AddComponent<RectRenderer>(50.f, 50.f);

	auto go2{scene.AddGameObject()};
	go2.AddComponent<Transform>(glm::vec2{50.f, 250.f}, 0.f);
	go2.AddComponent<RectRenderer>(50.f, 50.f);

	auto camera1{scene.AddGameObject()};
	camera1.AddComponent<Transform>(glm::vec2{0.f, 0.f}, 0.f);
	camera1.AddComponent<Camera>(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);

	auto camera2{scene.AddGameObject()};
	camera2.AddComponent<Transform>(glm::vec2{-50.f, 50.f}, 0.f);
	camera2.AddComponent<Camera>(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);

	SceneManager::GetInstance().SetActive(std::move(scene));

	roingine.Run([]() { GameEventQueue::GetInstance().Update(); });
}
