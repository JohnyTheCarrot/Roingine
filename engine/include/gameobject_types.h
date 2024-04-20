#ifndef GAMEOBJECT_TYPES_H
#define GAMEOBJECT_TYPES_H

#include "pair_hash.h"
#include <memory>
#include <unordered_map>

namespace roingine {
	class Component;

	using GameObjectHandle = size_t;
	using GameObjectComponents =
	        std::unordered_map<std::pair<GameObjectHandle, size_t>, std::unique_ptr<Component>, PairHash>;
}// namespace roingine

#endif// GAMEOBJECT_TYPES_H
