#include <luaT/luaT.h>
#include <CppBase/IteratorPair.h>
#include <Engine/GameScene.h>
#include <Engine/UserEventReceiverStack.h>
#include "../LuaEventReceiver.h"

// Don't remove this macro, or memory leak will occur
#define ITERATOR_USE_USERDATA

using namespace luaT;
using namespace xihad;
using namespace ngn;
namespace xihad { namespace script
{
	typedef IteratorPair<const GameScene::ObjectGroup> ObjectIterPair;

	// list, var
	static int objectIterator(lua_State* L)
	{
#ifndef ITERATOR_USE_USERDATA
		xassert(lua_islightuserdata(L, 1));
#else
		xassert(lua_isuserdata(L, 1) || lua_islightuserdata(L, 1));
#endif

		auto iterPtr = static_cast<ObjectIterPair*> (lua_touserdata(L, 1));

		if (iterPtr != nullptr && iterPtr->notEnd())
		{
			push<GameObject*>(L, *(*iterPtr));
			iterPtr->next();
		}
		else
		{
			lua_pushnil(L);
#ifndef ITERATOR_USE_USERDATA
			delete iterPtr;
#endif
		}

		return 1;
	}

	static int objectsWithTag(lua_State* L)
	{
		GameScene* scene = checkarg<GameScene*>(L, 1);
		const char* tag = checkarg<const char*>(L, 2);

		// Get list
		auto objList = scene->findObjectsByTag(tag);

		lua_pushcfunction(L, objectIterator);

		// Create Iterator Pair
		if (objList) 
		{
#ifdef ITERATOR_USE_USERDATA
			void* iterData = lua_newuserdata(L, sizeof(ObjectIterPair));

	#ifdef XIHAD_MLD_NEW_OBJECT
			// Enable __gc function when Auto-reference-counting enabled
			Metatable::forType<ObjectIterPair>(L);
			lua_setmetatable(L, -2);
	#endif

			new (iterData) ObjectIterPair(*objList);
#else
			lua_pushlightuserdata(L, new ObjectIterPair(*objList));
#endif
		}
		else
		{
			lua_pushlightuserdata(L, 0);
		}
		
		lua_pushnil(L);
		return 3;
	}

	static int requireSystem(lua_State* L)
	{
		GameScene* scene = checkarg<GameScene*>(L, 1);
		const std::string& sys = checkarg<const std::string&>(L, 2);
		scene->requireSystem(sys);

		// TODO return system
		return 0;
	}

	static int createObject(lua_State* L)
	{
		luaL_checkany(L, 2);
		GameScene* scene = checkarg<GameScene*>(L, 1);
		std::string& name = checkarg<std::string&>(L, 2);

		GameObject* obj;
		int n = lua_gettop(L);
		if (n == 2)
			obj = scene->createObject(name);
		else
			obj = scene->createObject(name, checkarg<GameObject*>(L, 3));

		push<GameObject*>(L, obj);
		return 1;
	}
	
	static int createUniqueObject(lua_State* L)
	{
		GameScene* scene = checkarg<GameScene*>(L, 1);
		std::string& header = checkarg<std::string&>(L, 2);

		GameObject* parent = NULL;
		bool addToTag = true;
		int n = lua_gettop(L);
		if (n >= 3)
		{
			parent = checkarg<GameObject*>(L, 3);
			if (n == 4)
				addToTag = checkarg<bool>(L, 4);
		}

		GameObject* obj = scene->createUniqueObject(header, parent, addToTag);

		push<GameObject*>(L, obj);
		return 1;
	}
	static int pushController(lua_State* L)
	{
		luaL_checkany(L, 2);
		GameScene* scene = checkarg<GameScene*>(L, 1);

		UserEventReceiver* receiver;
		if ((receiver = checkarg<UserEventReceiver*>(L, 2)) == 0)
		{
			LuaRef tref = LuaRef::fromIndex(L, 2);
			receiver = new LuaEventReceiver(tref);
			lua_pushvalue(L, 2);
		}
		
		scene->getControllerStack().pushReceiver(receiver);
		return 1;
	}

	luaT_static UserEventReceiver* popController(GameScene* scene)
	{
		auto ptr = scene->getControllerStack().popReceiver();
		if (ptr->getReferenceCount() == 1) 
			return 0;
		else
			return ptr.get();
	}}

	int luaopen_GameScene( lua_State* L )
	{
		luaT_defRegsBgn(receiver)
			luaT_cnnamefunc(intrusive_ptr_release<UserEventReceiver>, drop),
			luaT_cnnamefunc(intrusive_ptr_add_ref<UserEventReceiver>, grab),
		luaT_defRegsEnd
		MetatableFactory<UserEventReceiver>::create(L, receiver);

		luaT_defRegsBgn(sceneRegs)
			luaT_lnamedfunc(requireSystem),
			luaT_lnamedfunc(objectsWithTag),
			luaT_lnamedfunc(createObject),
			luaT_lnamedfunc(createUniqueObject),
			luaT_lnamedfunc(pushController),
			luaT_cnamedfunc(popController),
			luaT_mnamedfunc(GameScene, findObject), 
			luaT_mnamedfunc(GameScene, getDispatcher),
		luaT_defRegsEnd
		MetatableFactory<GameScene, CompositeUpdateHandler>::create(L, sceneRegs, 0);

#ifdef XIHAD_MLD_NEW_OBJECT
		MetatableFactory<ObjectIterPair>::createNull(L);
#endif

		return 0;
	}

}}

