#include "bomb.h"
#include "../audio.h"
#include "../event_queue.h"
#include "temporary_object.h"

#include <roingine/components/animation_renderer.h>
#include <roingine/components/transform.h>

namespace bomberman {
	constexpr float c_Size{90.f};

	void Bomb::OnCaughtInExplosion(event_queue::ExplosionData const &data) {

		if (auto const &transform{GetGameObject().GetComponent<roingine::Transform>()};
		    !data.IsInExplosion(transform.GetWorldPosition()))
			return;

		Explode();
	}

	Bomb::Bomb(roingine::GameObject gameObject, glm::vec2 location, PlayerInfo &bomber)
	    : Component{gameObject}
	    , m_rpBomber{&bomber}
	    , m_hExplosionHandler{
	              event_queue::EventQueue::GetInstance().AttachEventHandler<event_queue::EventType::Explosion>(
	                      [this](event_queue::ExplosionData const &data) { OnCaughtInExplosion(data); }
	              )
	      } {
		constexpr int   c_BombAnimationFrames{3};
		constexpr float c_SecondsPerFrame{0.5f};
		constexpr float c_DetonationTime{2.0f};

		gameObject.AddComponent<roingine::Transform>(location, 0.f);
		gameObject.AddComponent<roingine::AnimationRenderer>(
		        roingine::AnimationRenderer::AnimationInfo{
		                .fileName        = "res/img/bomb.png",
		                .numFrames       = c_BombAnimationFrames,
		                .secondsPerFrame = c_SecondsPerFrame
		        },
		        c_Size, c_Size, roingine::ScalingMethod::NearestNeighbor
		);
		gameObject.AddComponent<TemporaryObject>(c_DetonationTime, [this](roingine::GameObject) { Explode(); });

		audio::AudioServiceLocator::GetService().Play(audio::Sound::BombPlace);
	}

	void Bomb::Explode() {
		auto const &transform{GetGameObject().GetComponent<roingine::Transform>()};
		event_queue::EventQueue::GetInstance().FireEvent<event_queue::EventType::BombDetonated>(
		        m_rpBomber, transform.GetWorldPosition()
		);
		GetGameObject().Destroy();
	}
}// namespace bomberman
