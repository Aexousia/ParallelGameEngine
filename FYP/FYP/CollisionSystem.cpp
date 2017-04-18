#include "stdafx.h"
#include "CollisionSystem.h"
#include <CollisionComponent.h>
#include <VelocityComponent.h>

void CollisionSystem::process(float dt)
{
	auto cubes = AutoMap::getComponentGroups<CollisionSystem, AxisAlignedCubeCollisionComponent, TransformComponent>();
	
	glm::vec3 boundsMax, boundsMin;
	if (cubes.size())
	{
		auto& cube = cubes.front();
		TransformComponent* bodyTransform;
		AxisAlignedCubeCollisionComponent* bodyData;
		NULL_COMPONENT* _;
		std::tie(bodyData, bodyTransform, _, _, _) = cube;

		boundsMax = bodyData->maxPosition(bodyTransform);
		boundsMin = bodyData->minPosition(bodyTransform);
	}

	auto spheres = AutoMap::getComponentGroups<CollisionSystem, SphereCollisionComponent, TransformComponent, VelocityComponent>();
	for (int i = 0; i < spheres.size(); i++)
	{
		auto body1 = spheres[i];

		TransformComponent* body1Transform;
		SphereCollisionComponent* body1Data;
		VelocityComponent* body1Velocity;
		NULL_COMPONENT* _;
		std::tie(body1Data, body1Transform, body1Velocity, _, _) = body1;

		for (int j = i + 1; j < spheres.size(); j++)
		{
			auto body2 = spheres[j];
			
			TransformComponent* body2Transform;
			SphereCollisionComponent* body2Data;
			VelocityComponent* body2Velocity;
			NULL_COMPONENT* _;
			std::tie(body2Data, body2Transform, body2Velocity, _, _) = body2;

			glm::vec3	b1Pos = body1Transform->GetPos(), 
						b2Pos = body2Transform->GetPos(), 
						b2Tob1Dir = glm::normalize(b1Pos - b2Pos),
						b1Tob2Dir = glm::normalize(b2Pos - b1Pos);

			float	b1Radius = body1Data->getRadius(body1Transform), 
					b2Radius = body2Data->getRadius(body2Transform);

			float overlap = b1Radius + b2Radius - glm::distance(b1Pos, b2Pos);
			
			if (overlap > 0)
			{
				overlap *= 1.001f;
				body1Transform->SetPos(b1Pos + (b2Tob1Dir * overlap/2.f));
				//body2Transform->SetPos(b2Pos + (b1Tob2Dir * overlap / 2.f));
				body1Velocity->setVelocity(body2Velocity->getVelocity());
				//body2Velocity->setVelocity(body1Velocity->getVelocity());
			}


		}

		glm::vec3 bodyPos = body1Transform->GetPos();
		glm::vec3 bodyVelocity = body1Velocity->getVelocity();
		float bodyRadius = body1Data->getRadius(body1Transform);
		if (bodyPos.x > boundsMax.x - bodyRadius)
		{
			bodyPos.x = boundsMax.x - bodyRadius;
			bodyVelocity.x *= -1;
		}
		if (bodyPos.y > boundsMax.y - bodyRadius)
		{						  
			bodyPos.y = boundsMax.y - bodyRadius;
			bodyVelocity.y *= -1;
		}						  
		if (bodyPos.z > boundsMax.z - bodyRadius)
		{						  
			bodyPos.z = boundsMax.z - bodyRadius;
			bodyVelocity.z *= -1;
		}

		if (bodyPos.x < boundsMin.x + bodyRadius)
		{
			bodyPos.x = boundsMin.x + bodyRadius;
			bodyVelocity.x *= -1;
		}
		if (bodyPos.y < boundsMin.y + bodyRadius)
		{
			bodyPos.y = boundsMin.y + bodyRadius;
			bodyVelocity.y *= -1;
		}
		if (bodyPos.z < boundsMin.z + bodyRadius)
		{
			bodyPos.z = boundsMin.z + bodyRadius;
			bodyVelocity.z *= -1;
		}
		body1Transform->SetPos(bodyPos);
		body1Velocity->setVelocity(bodyVelocity);
	}
}
