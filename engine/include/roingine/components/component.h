#ifndef COMPONENT_H
#define COMPONENT_H

#include <type_traits>
#include <utility>

namespace roingine {
	class GameObject;

	class Component {
	public:
		using Handle = std::pair<std::size_t, std::size_t>;

		explicit Component(GameObject &gameObject)
		    : m_GameObject{gameObject} {
		}

		virtual ~Component(){};

		virtual void Update() = 0;

		virtual void FixedUpdate() = 0;

		virtual void Render() const = 0;

	protected:
		[[nodiscard]]
		GameObject &GetGameObject() {
			return m_GameObject;
		}

	private:
		GameObject &m_GameObject;
	};

	template<class T>
	concept ComponentImpl = std::derived_from<T, Component>;

	static_assert(std::is_abstract_v<Component>, "Component must be abstract");
}// namespace roingine

#endif// COMPONENT_H
