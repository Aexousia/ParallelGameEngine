#pragma once
#include <algorithm>

typedef unsigned int Change;
class IEntity;

static int newId()
{
	static int x = 0;
	return x++;
}

class ISystem
{
private:
	float m_timeSinceLastUpdate;
	float maxTimeTillUpdate;
public:
	ISystem(float ticksPerSecond = std::numeric_limits<float>::max()) : maxTimeTillUpdate(1.f / ticksPerSecond), m_timeSinceLastUpdate(0) {}

	virtual void SetTicksPerSecond(float ticksPerSecond)
	{
		maxTimeTillUpdate = (1.f / ticksPerSecond);
		m_timeSinceLastUpdate = 0;
	}

	virtual bool ready(float dt)
	{
		m_timeSinceLastUpdate += dt;
		if (m_timeSinceLastUpdate >= maxTimeTillUpdate)
		{
			m_timeSinceLastUpdate = 0;
			return true;
		}
		return false;
	}

	virtual void process(float dt) = 0;
};

struct IComponent
{
	IComponent() : id(newId()) {};

	virtual ~IComponent() {};
	virtual void ChangeOccured(Change c, IComponent* subject) {};
	virtual void setObservers(std::vector<IComponent*> observers)
	{

	}
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