#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <roingine/gameobject.h>

using duk_context = struct duk_hthread;

namespace roingine {
	class Scene final {
	public:
		Scene();
		~Scene();

		Scene(Scene &&);

		Scene &operator=(Scene &&);

		Scene(Scene const &) = delete;

		Scene &operator=(Scene const &) = delete;

		void PreUpdate();

		void Update();

		void PostUpdate();

		void FixedUpdate();

		void Render() const;

		[[nodiscard]]
		GameObject AddGameObject();

		void RegisterComponentType(std::string name, std::size_t hash, JSFactoryMapEntry::Function jsFactory);

	private:
		friend class GameObject;

		// TODO: pImpl seems unnecessary here, remove it
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}// namespace roingine

#endif// SCENE_H
