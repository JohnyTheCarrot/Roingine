#include <roingine/audio_service.h>
#include <roingine/commands/command.h>
#include <roingine/components/rect.h>
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

class Rotator final : public roingine::Component {
public:
	Rotator(GameObject &gameObject)
	    : Component{gameObject}
	    , m_Transform{GetGameObject().GetComponent<Transform>()} {
	}

	void Update() override {
		m_Transform.Rotate(.1f);
	}

	void FixedUpdate() override {
	}

	void Render() const override {
	}

private:
	Transform &m_Transform;
};

class KeyPressedCommand final : public roingine::Command {
public:
	~KeyPressedCommand() override = default;

	void Execute() override {
		std::cout << "key pressed" << std::endl;
	}
};

class KeyReleasedCommand final : public roingine::Command {
public:
	~KeyReleasedCommand() override = default;

	void Execute() override {
		std::cout << "key released" << std::endl;
	}
};

class KeyHeldCommand final : public roingine::Command {
public:
	~KeyHeldCommand() override = default;

	void Execute() override {
		std::cout << "key held" << std::endl;
	}
};

class KeyLongPressCommand final : public roingine::Command {
public:
	~KeyLongPressCommand() override = default;

	void Execute() override {
		std::cout << "key long press" << std::endl;
	}
};

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

int main() {
	roingine::Engine roingine{"Bubble Bobble", 640, 480};

	std::unordered_map<Sounds, std::string> soundMap{};
	soundMap.emplace(Sounds::TestSound, "sound.wav");

	AudioServiceLocator::Provide(std::make_unique<Audio>(std::move(soundMap)));

	KeyboardInput::Provide(std::make_unique<SDLKeyboardInputService>());
	KeyboardInput::GetService().AddCommand(
	        roingine::InputKeys::A, roingine::KeyEventType::Down, std::make_unique<KeyPressedCommand>()
	);
	KeyboardInput::GetService().AddCommand(
	        roingine::InputKeys::A, roingine::KeyEventType::Up, std::make_unique<KeyReleasedCommand>()
	);
	KeyboardInput::GetService().AddCommand(
	        roingine::InputKeys::A, roingine::KeyEventType::Held, std::make_unique<KeyHeldCommand>()
	);
	KeyboardInput::GetService().AddCommand(
	        roingine::InputKeys::A, roingine::KeyEventType::LongPress, std::make_unique<KeyLongPressCommand>()
	);

	Scene      scene{};
	GameObject parentGameObject{scene.AddGameObject()};
	std::reference_wrapper<Transform> pParentTransform{
	        parentGameObject.AddComponent<Transform>(glm::vec2{200.0f, 100.0f}, 0.0f)
	};
	parentGameObject.AddComponent<Rect>(50.f, 50.f);
	parentGameObject.AddComponent<RectRenderer>();

	for (int i{0}; i < 5; ++i) {
		GameObject go2{scene.AddGameObject()};
		auto      &pChildTransform{go2.AddComponent<Transform>(glm::vec2{50.0f, 50.0f}, 0.0f)};
		go2.AddComponent<Rect>(50.f, 50.f);
		go2.AddComponent<RectRenderer>();
		go2.AddComponent<Rotator>();
		pChildTransform.SetParent(pParentTransform.get());
		pParentTransform = pChildTransform;
	}

	GameEventQueue::GetInstance().AttachEventHandler<EventType::PlaySoundRequest>([](auto const &data) {
		AudioServiceLocator::GetService().Play(data.sound);
	});

	event_queue::EventQueue::GetInstance().AttachEventHandler<event_queue::EventType::CleanShutdown>([](auto const &) {
		std::cout << "Shutting down" << std::endl;
	});

	SceneManager::GetInstance().SetActive(std::move(scene));
	roingine.Run([]() {
		GameEventQueue::GetInstance().Update();
	});
}
