#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <type_traits>
#include <utility>

using duk_context = struct duk_hthread;

struct duk_function_list_entry;

namespace roingine {
	class GameObject;

	class Component {
	public:
		using Handle = std::pair<std::size_t, std::size_t>;

		explicit Component(std::reference_wrapper<GameObject> gameObject)
		    : m_GameObject{gameObject} {
		}

		virtual ~Component(){};

		virtual void PreUpdate() {
		}

		virtual void Update() = 0;

		virtual void PostUpdate() {
		}

		virtual void FixedUpdate() = 0;

		virtual void Render() const = 0;

		[[nodiscard]]
		virtual char const *GetName() const = 0;

		[[nodiscard]]
		virtual duk_function_list_entry const *SetUpScriptAPI(duk_context *ctx) const = 0;

	protected:
		friend class Scene;

		[[nodiscard]]
		GameObject &GetGameObject() {
			return m_GameObject;
		}

	private:
		std::reference_wrapper<GameObject> m_GameObject;
	};

	template<class T>
	concept ComponentImpl = requires(T comp) {
		std::derived_from<T, Component>;
		{
			T::JSFactory(static_cast<GameObject *>(nullptr), static_cast<duk_context *>(nullptr))
		} -> std::same_as<std::unique_ptr<T>>;
	};

	static_assert(std::is_abstract_v<Component>, "Component must be abstract");
}// namespace roingine

#endif// COMPONENT_H
