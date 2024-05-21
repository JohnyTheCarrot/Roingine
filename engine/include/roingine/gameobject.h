#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <optional>
#include <roingine/components/component.h>
#include <roingine/gameobject_types.h>
#include <string>

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

			RegisterTypeHashName(pComponent->GetName(), typeid(TComponent).hash_code());

			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponents{GetSceneComponents()};

			sceneComponents.insert({hComponent, std::move(pComponent)});
			return *rpComponent;
		}

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent &GetComponent() {
			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponents{GetSceneComponents()};

			if (sceneComponents.contains(hComponent))
				return *dynamic_cast<TComponent *>(sceneComponents.at(hComponent).get());

			throw ComponentNotFoundException{};
		}

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent const &GetComponent() const {
			Component::Handle hComponent{GetComponentHandle<TComponent>()};
			auto const       &sceneComponents{GetSceneComponents()};

			if (sceneComponents.contains(hComponent))
				return *dynamic_cast<TComponent *>(sceneComponents.at(hComponent).get());

			throw ComponentNotFoundException{};
		}

		[[nodiscard]]
		Component *GetComponent(std::size_t typeHash) {
			Component::Handle hComponent{m_hGameObject, typeHash};
			auto             &sceneComponents{GetSceneComponents()};

			if (sceneComponents.contains(hComponent))
				return sceneComponents.at(hComponent).get();

			throw ComponentNotFoundException{};
		}

		[[nodiscard]]
		Component *GetComponent(std::string const &name) {
			auto const hash{GetTypeHashFromName(name)};
			if (!hash.has_value())
				return nullptr;

			return GetComponent(hash.value());
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
		std::optional<std::size_t> GetTypeHashFromName(std::string const &name) const;

		void RegisterTypeHashName(std::string name, std::size_t hash);

		[[nodiscard]]
		GameObjectComponents const &GetSceneComponents() const noexcept;

		Scene *m_rpScene;
		Handle m_hGameObject;
	};
}// namespace roingine

#endif// GAMEOBJECT_H
