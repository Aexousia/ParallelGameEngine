#pragma once
#include <Framework.h>
#include <CollisionSystem.h>

namespace MassChanges
{
	static const Change mass = 1 << 1;
}

struct MassComponent : public IComponent, public AutoMapper<MassComponent, CollisionSystem>
{
	MassComponent(IEntity* parent, float _mass)
		:	SYSTEMS({
				SYSTEM(CollisionSystem)
			})
		,	IComponent(parent)
		,	mass(_mass)
	{
		Change x = Groupable<MassComponent>::bitId;
	}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		MassComponent* massComponent = static_cast<MassComponent*>(subject);
		if (change & MassChanges::mass)
		{
			mass = massComponent->mass;
		}
	}

	inline void SetMass(float newMass)
	{
		mass = newMass;
		SINGLETON(SyncManager)->registerChanges(this, MassChanges::mass);
	}

	float mass;
};