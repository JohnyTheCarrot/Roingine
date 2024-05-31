#include <roingine/audio_service.h>
#include <roingine/commands/command.h>
#include <roingine/components/rect.h>
#include <roingine/components/rect_collider.h>
#include <roingine/components/rect_renderer.h>
#include <roingine/components/scripts.h>
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
		m_Transform.Rotate(1.f);
	}

	void FixedUpdate() override {
	}

	void Render() const override {
	}

	[[nodiscard]]
	char const *GetName() const override {
		return "Rotator";
	}

	[[nodiscard]]
	duk_function_list_entry const *SetUpScriptAPI(duk_context *) const override {
		return nullptr;
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
	roingine::Engine roingine{"Script Test", 640, 480};

	std::unordered_map<Sounds, std::string> soundMap{};
	soundMap.emplace(Sounds::TestSound, "sound.wav");

	AudioServiceLocator::Provide(std::make_unique<Audio>(std::move(soundMap)));

	KeyboardInput::Provide(std::make_unique<SDLKeyboardInputService>());

	Scene      scene{};
	GameObject parentGameObject{scene.AddGameObject()};

	auto &scripts{parentGameObject.AddComponent<Scripts>()};
	scripts.AddScript(
	        "scripts/test.js",
	        [](std::string_view name, GameObject, std::vector<Script::DukValue> &&args) -> Script::DukValue {
		        if (name == "cppCallTest") {
			        std::cout << "cppCallTest called with " << args.size() << " args!" << std::endl;
			        return args.at(0);
		        }

		        return Script::DukUndefined{};
	        }
	);

	GameObject collider{scene.AddGameObject()};
	collider.AddComponent<Transform>(glm::vec2{300, 100}, 0.f);
	collider.AddComponent<Rect>(100.f, 100.f);
	collider.AddComponent<RectRenderer>();
	collider.AddComponent<RectCollider>(100.f, 100.f);

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
