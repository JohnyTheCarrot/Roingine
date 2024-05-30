#ifndef DUK_GAMEOBJECT_H
#define DUK_GAMEOBJECT_H

#include <roingine/duk_wrappers.h>
#include <string>

namespace roingine {
	class GameObject;
}

namespace roingine::duk_gameobject {
	void PushGameObject(GameObject *pGameObject, DukContext &ctx);

	void PutGameObject(DukObject &obj, GameObject *pGameObject, std::string const &key, DukContext &ctx);
}// namespace roingine::duk_gameobject

#endif// DUK_GAMEOBJECT_H
