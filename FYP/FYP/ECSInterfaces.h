#pragma once
#include <algorithm>
#include <vector>

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

	virtual float ready(float dt)
	{
		float timeSinceLastUpdate = m_timeSinceLastUpdate + dt;
		if (timeSinceLastUpdate >= maxTimeTillUpdate)
		{
			m_timeSinceLastUpdate = 0;
			return timeSinceLastUpdate + dt;
		}
		else
		{
			m_timeSinceLastUpdate = timeSinceLastUpdate;
			return 0;
		}
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