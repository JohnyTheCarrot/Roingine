#ifndef COMPONENT_H
#define COMPONENT_H

#include <gsl/pointers>
#include <type_traits>
#include <utility>

namespace roingine {
	class GameObject;

	class Component {
	public:
		using Handle = std::pair<std::size_t, std::size_t>;

		explicit Component(gsl::not_null<GameObject *> pGameObject)
		    : m_pGameObject{pGameObject} {
		}

		virtual ~Component(){};

		virtual void Update() = 0;

		virtual void FixedUpdate() = 0;

		virtual void Render() const = 0;

	protected:
		[[nodiscard]]
		gsl::not_null<GameObject *> GetGameObject() {
			return m_pGameObject;
		}

	private:
		gsl::not_null<GameObject *> m_pGameObject;
	};

	template<class T>
	concept ComponentImpl = std::derived_from<T, Component>;

	static_assert(std::is_abstract_v<Component>, "Component must be abstract");
}// namespace roingine

#endif// COMPONENT_H
