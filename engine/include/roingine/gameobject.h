#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <optional>
#include <roingine/component_init_types.h>
#include <roingine/gameobject_types.h>
#include <string>

using duk_context = struct duk_hthread;

namespace roingine {
	class GameObject;

	using ComponentHandle = std::pair<std::size_t, std::size_t>;

	template<class T>
	concept ComponentImpl = requires(T comp) {
		{
			T::JSFactory(static_cast<GameObject *>(nullptr), std::vector<JSData>{})
		} -> std::same_as<std::unique_ptr<T>>;
	};

	struct JSFactoryMapEntry final {
		using Function =
		        std::function<std::unique_ptr<Component>(GameObject *, std::vector<JSData> const &)>;

		Function    jsFactory;
	};

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

			ComponentHandle   hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponents{GetSceneComponents()};

			sceneComponents.insert({hComponent, std::move(pComponent)});
			return *rpComponent;
		}

		Component *AddComponent(std::string name, std::vector<JSData> const &args);

		void SetLabel(std::string &&label);

		[[nodiscard]]
		std::optional<std::string_view> GetLabel() const;

		void SetUniqueID(std::string &&uniqueID);

		[[nodiscard]]
		std::optional<std::string_view> GetUniqueID() const;

		void SetEnabled(bool enabled);

		[[nodiscard]]
		bool GetEnabled() const;

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent &GetComponent() {
			ComponentHandle   hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponents{GetSceneComponents()};

			if (sceneComponents.contains(hComponent))
				return *dynamic_cast<TComponent *>(sceneComponents.at(hComponent).get());

			throw ComponentNotFoundException{};
		}

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent const &GetComponent() const {
			ComponentHandle   hComponent{GetComponentHandle<TComponent>()};
			auto const       &sceneComponents{GetSceneComponents()};

			if (sceneComponents.contains(hComponent))
				return *dynamic_cast<TComponent *>(sceneComponents.at(hComponent).get());

			throw ComponentNotFoundException{};
		}

		[[nodiscard]]
		Component *GetComponent(std::size_t typeHash);

		[[nodiscard]]
		Component *GetOptionalComponent(std::size_t typeHash);

		[[nodiscard]]
		Component *GetComponent(std::string const &name);

		[[nodiscard]]
		Component *GetOptionalComponent(std::string const &name);

		template<ComponentImpl TComponent>
		[[nodiscard]]
		TComponent *GetOptionalComponent() noexcept {
			ComponentHandle   hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponent{GetSceneComponents()};

			if (sceneComponent.contains(hComponent))
				return dynamic_cast<TComponent *>(sceneComponent[hComponent].get());

			return nullptr;
		}

		template<ComponentImpl TComponent>
		void DestroyComponent() {
			ComponentHandle   hComponent{GetComponentHandle<TComponent>()};
			auto             &sceneComponent{GetSceneComponents()};

			sceneComponent.erase(hComponent);
		}

		void Destroy();

		[[nodiscard]]
		Handle GetHandle() const noexcept;

		void SetScene(Scene *scene) noexcept;

		[[nodiscard]]
		Scene *GetScene() const noexcept;

	private:
		template<ComponentImpl TComponent>
		[[nodiscard]]
		ComponentHandle GetComponentHandle() const noexcept {
			return {m_hGameObject, typeid(TComponent).hash_code()};
		}

		[[nodiscard]]
		std::optional<std::size_t> GetTypeHashFromName(std::string const &name) const;

		[[nodiscard]]
		std::optional<JSFactoryMapEntry> GetJSFactoryMapEntryByHash(std::size_t hash) const;

		[[nodiscard]]
		GameObjectComponents const &GetSceneComponents() const noexcept;

		[[nodiscard]]
		GameObjectComponents &GetSceneComponents() noexcept;

		Scene *m_rpScene;
		Handle m_hGameObject;
	};

	struct GameObjectHash final {
		std::size_t operator()(GameObject gameObject) const;
	};
}// namespace roingine

#endif// GAMEOBJECT_H
