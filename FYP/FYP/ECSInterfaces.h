#pragma once
#include "../dependancies/MetaSystem/MetaSystem.h"
#include <algorithm>

class IEntity;

struct IComponent : public AutoLister<IComponent>
{
	IComponent(int id) : ID(id) {};

	virtual ~IComponent() {};
	const int ID;
};

class IEntity : public AutoLister<IEntity>
{
public:
	IEntity(int id, std::vector<IComponent*> list) : ID(id), m_components(list)
	{
		alive = true;
	}

	virtual ~IEntity()
	{
		for (auto i : m_components)
		{
			delete i;
		}
		m_components.clear();
	}


	void AddComponent(IComponent* component)
	{
		m_components.push_back(component);
	}

	template<typename T>
	bool deleteComponent()
	{
		auto component = getComponentById<T>(ID);

		if (component)
		{
			delete component;
			m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
			return true;
		}
		return false;
	}

	const int ID;
	bool alive;
protected:
	std::vector<IComponent*> m_components;
};