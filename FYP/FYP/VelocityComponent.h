#pragma once
#include <Framework.h>
#include <../dependancies/glm/glm.hpp>
#include <PhysicsSystem.h>
#include <CollisionSystem.h>

namespace VelocityChanges
{
	static const Change velocity = 1 << 1;
}

struct VelocityComponent : public IComponent, public AutoMapper<VelocityComponent, PhysicsSystem, CollisionSystem>
{
	VelocityComponent(IEntity* parent, glm::vec3& vel)
		:	velocity(vel)
		,	IComponent(parent)
		,	SYSTEMS({
				SYSTEM(PhysicsSystem),
				SYSTEM_PRIORITY(CollisionSystem, Priority::High)
			})
	{

	}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		VelocityComponent* vel = static_cast<VelocityComponent*>(subject);
		if (change & VelocityChanges::velocity)
		{
			velocity = vel->getVelocity();
		}
	}

	inline glm::vec3 getVelocity()
	{
		return velocity;
	}

	inline void setVelocity(glm::vec3& vel)
	{
		velocity = vel;
		SINGLETON(SyncManager)->registerChanges(this, VelocityChanges::velocity);
	}

	glm::vec3 velocity;
};