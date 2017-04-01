#pragma once
#include "ECSInterfaces.h"

namespace AutoMap
{
	template<typename Component, typename System>
	std::vector<Component *>& get()
	{
		static std::pair<System*, std::vector<Component *>> instances = { nullptr, {} };
		return instances;
	}
}

template<typename self>
class ISystem : public Singleton<self>
{
	template<typename T>
	std::vector<T *>& getComponents()
	{
		return AutoMap::get<T, self>();
	}

	virtual void process(float dt) = 0;
};

// access components by id
// get list of objects
// associate all copies of objects with eachother
// create copies of objects for each system it wants by priority

//Automagically lists every object that inherits from it
//must pass type of self in as template param see: CRTP
//access list by calling AutoMap::get<TYPE>();
template<typename T>
class AutoMapper
{
public:
	AutoMapper()
	{
		AddElement<T>(static_cast<T *>(this));
	}

	AutoMapper(const AutoMapper& rhs)
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
	}

	template<typename T>
	static void RemoveElement(T* element)
	{
		auto& instances = AutoMap::get<T>();
		instances.erase(std::remove(instances.begin(), instances.end(), element), instances.end());
	}

	template<typename T>
	static void AddElement(T* element)
	{
		auto& instances = AutoMap::get<T>();
		instances.push_back(element);
	}

	virtual ~AutoMapper()
	{
		RemoveElement<T>(static_cast<T *>(this));
	}
};

//// components need systems by priority to prevent changes 
//// from a more important system happening simultaneously
//// automapper also needs to know all systems associated with the component type
//SYSTEMS_BY_PRIORITY(Component)
//{
//	SYSTEM(PhysicsSystem);
//	SYSTEM(RenderSystem);
//};