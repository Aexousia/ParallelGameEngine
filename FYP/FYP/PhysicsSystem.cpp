#include "stdafx.h"
#include "PhysicsSystem.h"
#include <VelocityComponent.h>
#include <TransformComponent.h>

void PhysicsSystem::process(float dt)
{
	auto bodies = AutoMap::getComponentGroups<PhysicsSystem, VelocityComponent, TransformComponent>();
	BATCH_LIST_BEGIN(bodies, 100, body, bodies, dt)
	{
		TransformComponent* bodyTransform;
		VelocityComponent* bodyVelocity;
		NULL_COMPONENT* _;
		std::tie(bodyVelocity, bodyTransform, _, _, _) = body;

		bodyTransform->SetPos(bodyTransform->GetPos() + bodyVelocity->getVelocity() * dt);
	}
	BATCH_LIST_END
}
