#include <roingine/components/rect_renderer.h>
#include <roingine/components/transform.h>
#include <roingine/roingine.h>
#include <roingine/scene.h>
#include <roingine/scene_manager.h>

using namespace roingine;

class Rotator final : public roingine::Component {
public:
	Rotator(GameObject *pGameObject)
	    : Component{pGameObject}
	    , m_pTransform{GetGameObject()->GetComponent<Transform>()} {
	}

	void Update() {
		m_pTransform->Rotate(.1f);
	}

	void FixedUpdate() {
	}

	void Render() const {
	}

private:
	Transform *m_pTransform;
};

int main() {
	roingine::Engine::Settings roingineSettings{.gameTitle = "Bubble Bobble", .windowWidth = 640, .windowHeight = 480};
	roingine::Engine           roingine{roingineSettings};

	Scene      scene{};
	GameObject parentGameObject{scene.AddGameObject()};
	auto       pParentTransform{parentGameObject.AddComponent<Transform>(glm::vec2{200.0f, 100.0f}, 0.0f)};
	parentGameObject.AddComponent<RectRenderer>(100.f, 100.f);

	for (int i{0}; i < 5; ++i) {
		GameObject go2{scene.AddGameObject()};
		auto       pChildTransform{go2.AddComponent<Transform>(glm::vec2{50.0f, 50.0f}, 0.0f)};
		go2.AddComponent<RectRenderer>(50.f, 50.f);
		// pChildTransform->SetPivot(25.0f, 25.0f);
		go2.AddComponent<Rotator>();
		pChildTransform->SetParent(pParentTransform);
		pParentTransform = pChildTransform;
	}


	SceneManager::GetInstance().SetActive(std::move(scene));
	roingine.Run();
}
