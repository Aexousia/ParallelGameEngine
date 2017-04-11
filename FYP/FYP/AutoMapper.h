#pragma once
#include "ECSInterfaces.h"
#include <algorithm>
#include <unordered_map>
#include <assert.h>
#include "SyncManager.h"

//helper function
template <typename Key, typename Value>
Value& get_or(std::unordered_map<Key, Value>& m, const Key& key, Value& default_value)
{
	auto it = m.find(key);
	if (it == m.end()) {
		return default_value;
	}
	return it->second;
}

namespace AutoMap
{
	template<typename Component, typename System>
	std::vector<Component *>& get()
	{
		static std::vector<Component *> instances;
		return instances;
	}
}

template<typename self>
class IAutoMapUser : public Singleton<self>
{
public:
};

class NULL_SYSTEM : public ISystem
{
public:
	static NULL_SYSTEM* getInstance()
	{
		return nullptr;
	}
};

//Automagically lists every object that inherits from it
//must pass type of self in as template param see: CRTP
//access list by calling AutoMap::get<TYPE>();
template<	typename Component, 
			typename SYSTEM1, //one system is needed at minimum
			typename SYSTEM2 = NULL_SYSTEM,  //more systems are optional
			typename SYSTEM3 = NULL_SYSTEM, 
			typename SYSTEM4 = NULL_SYSTEM>
class AutoMapper
{
public:
	AutoMapper(std::unordered_map<ISystem*, int> priorityMap)
	{
		std::vector<IComponent*> copies;
		Component* c = static_cast<Component *>(this);
		Component* original = c;

		auto system = SINGLETON(SYSTEM1);
		assert(system);
		AddElement<SYSTEM1>(c);
		c->priority = get_or<ISystem*>(priorityMap, system, Priority::Normal);
		copies.push_back(c);

		auto system2 = SINGLETON(SYSTEM2);
		if (system2)
		{
			c = new Component(*c);
			c->priority = get_or<ISystem*>(priorityMap, system2, Priority::Normal);
			AddElement<SYSTEM2>(c);
			copies.push_back(c);
		}

		auto system3 = SINGLETON(SYSTEM3);
		if (system3)
		{
			c = new Component(*c);
			c->priority = get_or<ISystem*>(priorityMap, system3, Priority::Normal);
			AddElement<SYSTEM3>(c);
			copies.push_back(c);
		}

		auto system4 = SINGLETON(SYSTEM4);
		if (system4)
		{
			c = new Component(*c);
			c->priority = get_or<ISystem*>(priorityMap, system4, Priority::Normal);
			AddElement<SYSTEM4>(c);
			copies.push_back(c);
		}

		//Sync manager needs to know who to distribute changes to
		SINGLETON(SyncManager)->addRecipients(copies, c->id);
		SINGLETON(SyncManager)->registerChanges(original, static_cast<Change>(-1));//constructor gets called first, so distribution must be run on all changes
	}

	/*AutoMapper(const AutoMapper& rhs)
	{
		AddElement<T>(static_cast<T *>(this));
	}

	AutoMapper(AutoMapper&& rhs)
	{
		AddElement<T>(static_cast<T *>(this));
	}

	AutoMapper& operator=(const AutoMapper& rhs)
	{
		AddElement<T>(static_cast<T *>(this));
		return *this;
	}*/

	template<typename System>
	static void RemoveElement(int elementId, Component* deleted)
	{
		auto& instances = AutoMap::get<Component, System>();
		std::for_each(instances.begin(), instances.end(), [elementId, deleted](Component*& c)
		{ 
			if (c->id == elementId)
			{
				if (c != deleted)
				{
					delete c;
				}
				
				c = nullptr;
			}; 
		});
		instances.erase(std::remove(instances.begin(), instances.end(), nullptr), instances.end());
	}

	template<typename System>
	static void AddElement(Component* element)
	{
		auto& instances = AutoMap::get<Component, System>();
		instances.push_back(element);
	}

	virtual ~AutoMapper()
	{
		static int lastRemoved = 0; // the previously removed element, prevent multiple destructor calls for same object
		Component* deletedComponent = static_cast<Component *>(this);
		int elementId = deletedComponent->id; // the id of the object to destroy in all system's copies

		if (elementId == lastRemoved)
		{
			return;
		}

		lastRemoved = elementId;

		//Cleanup memory related to component
		SINGLETON(SyncManager)->removeRecipients(elementId);

		RemoveElement<SYSTEM1>(elementId, deletedComponent);

		if (SINGLETON(SYSTEM2))
		{
			RemoveElement<SYSTEM2>(elementId, deletedComponent);
		}

		if (SINGLETON(SYSTEM3))
		{
			RemoveElement<SYSTEM3>(elementId, deletedComponent);
		}

		if (SINGLETON(SYSTEM4))
		{
			RemoveElement<SYSTEM4>(elementId, deletedComponent);
		}
	}
};

namespace Priority
{
	static int VeryHigh = 0;
	static int High = 1;
	static int Normal = 2;
	static int Low = 3;
	static int VeryLow = 4;
}


#define SYSTEM_PRIORITY(name, priority) { SINGLETON(name), priority }
#define SYSTEM(name) { SINGLETON(name), Priority::Normal }
#define SYSTEMS AutoMapper

//// components need systems by priority to prevent changes 
//// from a more important system happening simultaneously
//// automapper also needs to know all systems associated with the component type
//SYSTEMS_BY_PRIORITY(Component)
//{
//	SYSTEM(PhysicsSystem);
//	SYSTEM(RenderSystem);
//};