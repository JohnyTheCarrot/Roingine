#ifndef COMPONENT_H
#define COMPONENT_H

#include <roingine/gameobject.h>
#include <type_traits>
#include <utility>

using duk_context = struct duk_hthread;

struct duk_function_list_entry;

namespace roingine {
	class Component {
	public:
		using Handle = std::pair<std::size_t, std::size_t>;

		explicit Component(GameObject gameObject)
		    : m_GameObject{gameObject} {
		}

		Component(Component &&other) noexcept;

		virtual ~Component() = default;

		virtual void PreUpdate() {
		}

		virtual void Update() = 0;

		virtual void PostUpdate() {
		}

		virtual void FixedUpdate() = 0;

		virtual void Render() const = 0;

		virtual void OnEnabled() {
		}

		virtual void OnDisabled() {
		}

		friend class Scene;

		[[nodiscard]]
		GameObject &GetGameObject() {
			return m_GameObject;
		}

	private:
		GameObject m_GameObject;
	};

	static_assert(std::is_abstract_v<Component>, "Component must be abstract");
}// namespace roingine

#endif// COMPONENT_H
