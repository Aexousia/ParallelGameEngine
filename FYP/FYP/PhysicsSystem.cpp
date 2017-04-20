#include "stdafx.h"
#include "PhysicsSystem.h"
#include <VelocityComponent.h>
#include <TransformComponent.h>

void PhysicsSystem::process(float dt)
{
	/*auto bodies = AutoMap::getComponentGroups<PhysicsSystem, VelocityComponent, TransformComponent>();
	for (auto body : bodies)
	{
		TransformComponent* bodyTransform;
		VelocityComponent* bodyVelocity;
		NULL_COMPONENT* _;
		std::tie(bodyVelocity, bodyTransform, _, _, _) = body;

		bodyTransform->SetPos(bodyTransform->GetPos() + bodyVelocity->getVelocity() * dt);
	}*/
}
