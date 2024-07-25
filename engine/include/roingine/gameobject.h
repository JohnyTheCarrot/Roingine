#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <functional>
#include <optional>
#include <roingine/component_init_types.h>
#include <roingine/gameobject_types.h>
#include <string>

using duk_context = struct duk_hthread;

namespace roingine {
	class GameObject;

	using ComponentHandle = std::size_t;

	struct JSFactoryMapEntry final {
		using Function = std::function<std::unique_ptr<Component>(GameObject *, std::vector<JSData> const &)>;

		Function jsFactory;
	};

	class Scene;

	class ComponentNotFoundException final : public std::exception {
	public:
		[[nodiscard]]
		char const *what() const noexcept override {
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
		        class TComponent, class... Args,
		        std::enable_if_t<std::is_constructible_v<TComponent, GameObject &, Args &&...>, bool> = true>
		TComponent &AddComponent(Args &&...args) {
			if (auto *existing{GetOptionalComponent<TComponent>()}; existing != nullptr)
				return *existing;

			std::unique_ptr<TComponent> pComponent{std::make_unique<TComponent>(*this, std::forward<Args>(args)...)};
			TComponent                 *rpComponent{pComponent.get()};

			ComponentHandle hComponent{GetComponentHandle<TComponent>()};
			auto           &sceneComponents{GetOrCreateComponentList()};

			sceneComponents.insert({hComponent, std::move(pComponent)});
			return *rpComponent;
		}

		void SetLabel(std::string &&label) const;

		[[nodiscard]]
		std::optional<std::string_view> GetLabel() const;

		void SetUniqueID(std::string &&uniqueID) const;

		[[nodiscard]]
		std::optional<std::string_view> GetUniqueID() const;

		void SetEnabled(bool enabled);

		[[nodiscard]]
		bool GetEnabled() const;

		template<class TComponent>
		[[nodiscard]]
		TComponent &GetComponent() {
			ComponentHandle const hComponent{GetComponentHandle<TComponent>()};

			if (auto const sceneComponents{GetComponents()}; sceneComponents != nullptr) {
				if (auto const it{sceneComponents->find(hComponent)}; it != sceneComponents->end())
					return *dynamic_cast<TComponent *>(it->second.get());
			}

			throw ComponentNotFoundException{};
		}

		template<class TComponent>
		[[nodiscard]]
		TComponent const &GetComponent() const {
			ComponentHandle const hComponent{GetComponentHandle<TComponent>()};

			if (auto const *sceneComponents{GetComponents()}; sceneComponents != nullptr) {
				auto it{sceneComponents->find(hComponent)};
				if (it != sceneComponents->cend())
					return *dynamic_cast<TComponent *>(it);
			}

			throw ComponentNotFoundException{};
		}

		[[nodiscard]]
		Component *GetComponent(std::size_t typeHash);

		[[nodiscard]]
		Component *GetOptionalComponent(std::size_t typeHash);

		template<class TComponent>
		[[nodiscard]]
		TComponent *GetOptionalComponent() noexcept {
			ComponentHandle const hComponent{GetComponentHandle<TComponent>()};

			if (auto const sceneComponents{GetComponents()}; sceneComponents != nullptr) {
				if (auto const it{sceneComponents->find(hComponent)}; it != sceneComponents->end())
					return dynamic_cast<TComponent *>(it->second.get());
			}

			return nullptr;
		}

		template<class TComponent>
		void DestroyComponent() {
			ComponentHandle const hComponent{GetComponentHandle<TComponent>()};
			auto                 &sceneComponent{GetSceneComponents()};

			sceneComponent.erase(hComponent);
		}

		void Destroy();

		[[nodiscard]]
		Handle GetHandle() const noexcept;

		void SetScene(Scene *scene) noexcept;

		[[nodiscard]]
		Scene *GetScene() const noexcept;

	private:
		[[nodiscard]]
		ComponentMap *GetComponents();

		[[nodiscard]]
		ComponentMap const *GetComponents() const;

		[[nodiscard]]
		ComponentMap &GetOrCreateComponentList();

		template<class TComponent>
		[[nodiscard]]
		static ComponentHandle GetComponentHandle() noexcept {
			return typeid(TComponent).hash_code();
		}

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
