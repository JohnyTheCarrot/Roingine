#ifndef GAMEOBJECT_TYPES_H
#define GAMEOBJECT_TYPES_H

#include <memory>
#include <unordered_map>

namespace roingine {
	class Component;

	using GameObjectHandle     = std::size_t;
	using ComponentMap         = std::unordered_map<std::size_t, std::unique_ptr<Component>>;
	using GameObjectComponents = std::unordered_map<GameObjectHandle, ComponentMap>;
}// namespace roingine

#endif// GAMEOBJECT_TYPES_H
