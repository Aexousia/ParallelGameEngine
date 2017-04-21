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
	snapToBounds(boundsMin, boundsMax, spheres);	
	findCollidingPairs(spheres);
	resolveCollidingPairs();
}

void CollisionSystem::snapToBounds(glm::vec3& boundsMin, glm::vec3& boundsMax, std::vector<SphereBodyData>& spheres)
{
	for (int i = 0; i < spheres.size(); i++)
	{
		auto body1 = spheres[i];

		TransformComponent* body1Transform;
		SphereCollisionComponent* body1Data;
		VelocityComponent* body1Velocity;
		NULL_COMPONENT* _;
		std::tie(body1Data, body1Transform, body1Velocity, _, _) = body1;

		bool b1CollidingWithWall = false;
		glm::vec3 bodyPos = body1Transform->GetPos();
		glm::vec3 bodyVelocity = body1Velocity->getVelocity();
		float bodyRadius = body1Data->getRadius(body1Transform);
		if (bodyPos.x > boundsMax.x - bodyRadius)
		{
			bodyPos.x = boundsMax.x - bodyRadius;
			bodyVelocity.x = std::abs(bodyVelocity.x) * -1;
		}
		if (bodyPos.y > boundsMax.y - bodyRadius)
		{
			bodyPos.y = boundsMax.y - bodyRadius;
			bodyVelocity.y = std::abs(bodyVelocity.y) * -1;
		}
		if (bodyPos.z > boundsMax.z - bodyRadius)
		{
			bodyPos.z = boundsMax.z - bodyRadius;
			bodyVelocity.z = std::abs(bodyVelocity.z) * -1;
		}

		if (bodyPos.x < boundsMin.x + bodyRadius)
		{
			bodyPos.x = boundsMin.x + bodyRadius;
			bodyVelocity.x = std::abs(bodyVelocity.x);
		}
		if (bodyPos.y < boundsMin.y + bodyRadius)
		{
			bodyPos.y = boundsMin.y + bodyRadius;
			bodyVelocity.y = std::abs(bodyVelocity.y);
		}
		if (bodyPos.z < boundsMin.z + bodyRadius)
		{
			bodyPos.z = boundsMin.z + bodyRadius;
			bodyVelocity.z = std::abs(bodyVelocity.z);
		}
		body1Transform->SetPos(bodyPos);
		body1Velocity->setVelocity(bodyVelocity);
	}
}

void CollisionSystem::resolveCollidingPairs()
{
	for (auto& collision : m_collidingPairs)
	{
		TransformComponent* body1Transform;
		SphereCollisionComponent* body1Data;
		VelocityComponent* body1Velocity;
		NULL_COMPONENT* _;
		std::tie(body1Data, body1Transform, body1Velocity, _, _) = collision.body1;

		TransformComponent* body2Transform;
		SphereCollisionComponent* body2Data;
		VelocityComponent* body2Velocity;
		std::tie(body2Data, body2Transform, body2Velocity, _, _) = collision.body2;

		glm::vec3	b1Pos = body1Transform->GetPos(),
					b2Pos = body2Transform->GetPos();

		glm::vec3 b2Tob1Dir = glm::normalize(b1Pos - b2Pos);

		if (b1Pos == b2Pos)
		{
			b2Tob1Dir = glm::vec3(1, 0, 0);
			glm::rotate(rand() % 628 / 100.f, b2Tob1Dir);
		}

		body1Transform->SetPos(body1Transform->GetPos() + b2Tob1Dir * collision.overlap / 2.f);
		body2Transform->SetPos(body2Transform->GetPos() - b2Tob1Dir * collision.overlap / 2.f);

		glm::vec3 newBody1Velocity = body1Velocity->getVelocity(), newBody2Velocity = body2Velocity->getVelocity();

		if (glm::dot(-b2Tob1Dir, body1Velocity->getVelocity()))
		{
			newBody1Velocity = newBody2Velocity;
		}
		if (glm::dot(b2Tob1Dir, body1Velocity->getVelocity()))
		{
			newBody2Velocity = body1Velocity->getVelocity();
		}
		if (body1Velocity->getVelocity() != newBody1Velocity)
		{
			body1Velocity->setVelocity(newBody1Velocity);
		}

		if (body2Velocity->getVelocity() != newBody2Velocity)
		{
			body2Velocity->setVelocity(newBody2Velocity);
		}
	}

	m_collidingPairs.clear();
}

void CollisionSystem::findCollidingPairs(std::vector<SphereBodyData>& spheres)
{
	std::vector<std::pair<SphereBodyData&, SphereBodyData&>> m_bodyPairs;
	//N(N-1)/2
	m_bodyPairs.reserve((spheres.size() * (spheres.size() - 1)) / 2);
	for (int i = 0; i < spheres.size(); i++)
	{
		for (int j = i + 1; j < spheres.size(); j++)
		{
			m_bodyPairs.push_back({spheres[i], spheres[j]});
		}
	}

	for (auto& pair : m_bodyPairs)
	{
		pairCollidingCheck(pair.first, pair.second);
	}
}

void CollisionSystem::pairCollidingCheck(SphereBodyData& body1, SphereBodyData& body2)
{
	TransformComponent* body1Transform;
	SphereCollisionComponent* body1Data;
	VelocityComponent* body1Velocity;
	NULL_COMPONENT* _;
	std::tie(body1Data, body1Transform, body1Velocity, _, _) = body1;

	TransformComponent* body2Transform;
	SphereCollisionComponent* body2Data;
	VelocityComponent* body2Velocity;
	std::tie(body2Data, body2Transform, body2Velocity, _, _) = body2;

	glm::vec3	b1Pos = body1Transform->GetPos(),
				b2Pos = body2Transform->GetPos();


	float	b1Radius = body1Data->getRadius(body1Transform),
			b2Radius = body2Data->getRadius(body2Transform);

	glm::vec3 dist = b1Pos - b2Pos;
	float distX = dist.x * dist.x, distY = dist.y * dist.y, distZ = dist.z * dist.z;
	float distSquared = distX + distY + distZ;
	float radSquared = (b1Radius + b2Radius) * (b1Radius + b2Radius);
	bool colliding = radSquared > distSquared;

	if (colliding)
	{
		float overlap = std::sqrt(radSquared - distSquared);

		m_collidingPairs.push_back(CollisionPair(body1, body2, overlap));
	}
}
