#pragma once
#include <algorithm>

typedef unsigned int Change;
class IEntity;

static int newId()
{
	static int x = 0;
	return x++;
}

struct IComponent
{
	IComponent(IEntity* parent) : id(newId()) {};

	virtual ~IComponent() {};
	virtual void ChangeOccured(Change c, IComponent* subject) {};
	virtual void setObservers(std::vector<IComponent*> observers)
	{

	}
	IEntity* parent;
	int id;
	int priority;
};

class IEntity
{
public:
	IEntity(std::vector<IComponent*> list) : m_components(list)
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



	bool alive;
protected:
	std::vector<IComponent*> m_components;
};