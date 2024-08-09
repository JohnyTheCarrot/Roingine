#ifndef COMPONENT_H
#define COMPONENT_H

#include <roingine/gameobject.h>
#include <utility>

namespace roingine {
	class Component {
	public:
		using Handle = std::pair<std::size_t, std::size_t>;

		explicit Component(GameObject gameObject);

		Component(Component &&other) noexcept;

		virtual ~Component();

		virtual void PreUpdate() {
		}

		virtual void Update() {
		}

		virtual void PostUpdate() {
		}

		virtual void FixedUpdate() {
		}

		virtual void Render() const {
		}

		virtual void OnEnabled() {
		}

		virtual void OnDisabled() {
		}

		friend class Scene;

		[[nodiscard]]
		GameObject &GetGameObject();

		[[nodiscard]]
		GameObject const &GetGameObject() const;

		[[nodiscard]]
		bool GetIsEnabled() const noexcept;

		void SetEnabled(bool isEnabled) noexcept;

	private:
		GameObject m_GameObject;
		bool       m_IsEnabled{true};
	};
}// namespace roingine

#endif// COMPONENT_H
