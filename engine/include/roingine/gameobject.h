#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <roingine/components/component.h>
#include <roingine/gameobject_types.h>

namespace roingine {
	class Scene;

	class ComponentNotFoundException final : public std::exception {
	public:
		char const *what() const override {
			return "component not found";
		}
	};

	class GameObject final {
	public:
		using Handle = size_t;

		GameObject(Scene &scene, Handle hGoHandle);

		[[nodiscard]]
		bool
		operator==(GameObject const &other) const noexcept;

		template<
		        ComponentImpl TComponent, class... Args,
		        std::enable_if_t<std::is_constructible_v<TComponent, GameObject &, Args &&...>, bool> = true>
		TComponent &AddComponent(Args &&...args) {
			if (auto *existing{GetOptionalComponent<TComponent>()}; existing != nullptr)
				return *existing;

			std::unique_ptr<TComponent> pComponent{std::make_unique<TComponent>(*this, std::forward<Args>(args)...)};
			TComponent                 *rpComponent{pComponent.get()};

			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponents{GetSceneComponents()};

			sceneComponents.insert({hComponent, std::move(pComponent)});
			return *rpComponent;
		}

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent &GetComponent() {
			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponent{GetSceneComponents()};

			if (sceneComponent.contains(hComponent))
				return *dynamic_cast<TComponent *>(sceneComponent.at(hComponent).get());

			throw ComponentNotFoundException{};
		}

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent const &GetComponent() const {
			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto const       &sceneComponent{GetSceneComponents()};

			if (sceneComponent.contains(hComponent))
				return *dynamic_cast<TComponent *>(sceneComponent.at(hComponent).get());

			throw ComponentNotFoundException{};
		}

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent *GetOptionalComponent() noexcept {
			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponent{GetSceneComponents()};

			if (sceneComponent.contains(hComponent))
				return dynamic_cast<TComponent *>(sceneComponent[hComponent].get());

			return nullptr;
		}

		template<ComponentImpl TComponent>
		void DestroyComponent() {
			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponent{GetSceneComponents()};

			sceneComponent.erase(hComponent);
		}

	private:

		template<ComponentImpl TComponent>
		[[nodiscard]]
		Component::Handle GetComponentHandle() const noexcept {
			return {m_hGameObject, typeid(TComponent).hash_code()};
		}

		[[nodiscard]]
		GameObjectComponents &GetSceneComponents() noexcept;

		[[nodiscard]]
		GameObjectComponents const &GetSceneComponents() const noexcept;

		Scene *m_rpScene;
		Handle m_hGameObject;
	};
}// namespace roingine

#endif// GAMEOBJECT_H
