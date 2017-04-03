#pragma once
#include "ECSInterfaces.h"
#include <algorithm>

namespace AutoMap
{
	template<typename Component, typename System>
	std::vector<Component *>& get()
	{
		static std::pair<System*, std::vector<Component *>> instances = { nullptr, {} };
		return instances.second;
	}
}

template<typename self>
class IAutoMapUser : public Singleton<self>
{
	template<typename T>
	std::vector<T *>& getComponents()
	{
		return AutoMap::get<T, self>();
	}
};


class ISystem 
{
	virtual void process(float dt) = 0;
};

class NULL_SYSTEM : public ISystem
{
public:
	static NULL_SYSTEM* getInstance()
	{
		return nullptr;
	}
};

// access components by id
// get list of objects
// associate all copies of objects with eachother
// create copies of objects for each system it wants by priority

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
private:
	std::vector<std::pair<ISystem*, int>> m_priorities;
public:
	AutoMapper(std::vector<std::pair<ISystem*, int>> priorityMap)
		:	m_priorities(priorityMap)
	{
		Component* c = static_cast<Component *>(this);

		assert(SINGLETON(SYSTEM1));
		AddElement<SYSTEM1>(c);

		if (SINGLETON(SYSTEM2))
		{
			AddElement<SYSTEM2>(new Component(*c));
		}

		if (SINGLETON(SYSTEM3))
		{
			AddElement<SYSTEM3>(new Component(*c));
		}

		if (SINGLETON(SYSTEM4))
		{
			AddElement<SYSTEM4>(new Component(*c));
		}
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
	static void RemoveElement(int componentId, Component* deleted)
	{
		auto& instances = AutoMap::get<Component, System>();
		std::for_each(instances.begin(), instances.end(), [componentId, deleted](Component*& c)
		{ 
			if (c->ID == componentId)
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
		int elementId = deletedComponent->ID; // the id of the object to destroy in all system's copies

		if (elementId == lastRemoved)
		{
			return;
		}

		lastRemoved = elementId;

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


#define SYSTEM(name, priority) { SINGLETON(name), priority }
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