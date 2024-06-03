#ifndef DUK_GAMEOBJECT_H
#define DUK_GAMEOBJECT_H

#include <roingine/duk_wrappers.h>
#include <string>

namespace roingine {
	class GameObject;
}

using duk_context = struct duk_hthread;

namespace roingine::duk_gameobject {
	void PushGameObject(GameObject const &pGameObject, duk_context *);

	void PutGameObject(DukObject &obj, GameObject const &pGameObject, std::string const &key, DukContext &ctx);
}// namespace roingine::duk_gameobject

#endif// DUK_GAMEOBJECT_H
