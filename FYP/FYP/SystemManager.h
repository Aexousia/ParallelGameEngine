#pragma once
#include "ECSInterfaces.h"
#include "Singleton.h"
#include <vector>

class SystemManager : public Singleton<SystemManager>
{
public:
	void runSystems(float dt)
	{
		for (auto& system : m_systems)
		{
			SINGLETON(TaskQueue)->addJob(std::bind([system, dt]() {
				if (system->ready(dt))
				{
					system->process(dt);
				}
			}));
		}
	}

	void registerSystem(ISystem* system)
	{
		m_systems.push_back(system);
	}

	void deregisterSystem(ISystem* system)
	{
		m_systems.erase(std::remove(m_systems.begin(), m_systems.end(), system), m_systems.end());
		delete system;
	}

private:
	std::vector<ISystem*> m_systems;
};

#define REGISTER_SYSTEM_TICK_RATE(name, ticksPerSecond) SINGLETON(name)->SetTicksPerSecond(ticksPerSecond); SINGLETON(SystemManager)->registerSystem(SINGLETON(name))
#define REGISTER_SYSTEM(name) SINGLETON(SystemManager)->registerSystem(SINGLETON(name))