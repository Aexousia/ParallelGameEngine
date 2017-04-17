#pragma once
#include <algorithm>
#include <vector>

typedef unsigned int Change;
class IEntity;

inline int newId()
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

inline unsigned int uniqueBitId()
{
	static unsigned int offset = 0;
	offset++;
	return 1 << offset;
}

template<typename Component>
struct Groupable
{
	static unsigned int bitId;
};
template<typename Component>
unsigned int Groupable<Component>::bitId = uniqueBitId();

struct NULL_COMPONENT : public Groupable<NULL_COMPONENT>
{};


struct IComponent
{
	IComponent(IEntity* parent) 
		:	parent(parent)
		,	id(newId()) 
	{};

	virtual ~IComponent() {};
	virtual void ChangeOccured(Change c, IComponent* subject) {};
	virtual void setObservers(std::vector<IComponent*> observers)
	{

	}
	int id;
	int priority;
	IEntity* parent;
};

class IEntity
{
public:
	IEntity()
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

	template<typename Component>
	void AddComponent(IComponent* component)
	{
		m_components.push_back(component);

		//allows easy grouping of components later
		m_componentBitMask |= Groupable<Component>::bitId;
	}

	unsigned int getComponentBitMask()
	{
		return m_componentBitMask;
	}
	
	bool alive;
protected:
	std::vector<IComponent*> m_components;
	unsigned int m_componentBitMask;
};