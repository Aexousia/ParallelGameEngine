#pragma once
#include <../dependancies/glm/glm.hpp>
#include <TransformComponent.h>
#include <CollisionSystem.h>

class ICollisionComponent : public IComponent
{
public:
	ICollisionComponent(IEntity* parent)
		:	IComponent(parent)
	{

	}
};

class AxisAlignedCubeCollisionComponent : public ICollisionComponent, public AutoMapper<AxisAlignedCubeCollisionComponent, CollisionSystem>
{
public:
	AxisAlignedCubeCollisionComponent(IEntity* parent, float defaultModelHeight=2)
		:	ICollisionComponent(parent)
		,	m_defaultSize(defaultModelHeight)
		,	SYSTEMS({
				SYSTEM(CollisionSystem)
			})
	{
	}

	glm::vec3 minPosition(TransformComponent const* t)
	{
		float offset = m_defaultSize / 2.f;
		glm::vec4 temp = glm::vec4(-offset, -offset, -offset, 1);
		glm::vec3 minPos = t->GetModel() * temp;

		return minPos;
	}

	glm::vec3 maxPosition(TransformComponent const* t)
	{
		float offset = m_defaultSize / 2.f;
		glm::vec4 temp = glm::vec4(offset, offset, offset, 1);
		glm::vec3 minPos = t->GetModel() * temp;

		return minPos;
	}
private:
	float m_defaultSize;
};

class SphereCollisionComponent : public ICollisionComponent, public AutoMapper<SphereCollisionComponent, CollisionSystem>
{
public:
	SphereCollisionComponent(IEntity* parent, float defaultRadius = 1)
		:	ICollisionComponent(parent)
		,	m_defaultRadius(defaultRadius)
		,	SYSTEMS({
				SYSTEM(CollisionSystem)
			})
	{
	}

	float getRadius(TransformComponent const* t)
	{
		return m_defaultRadius * t->GetScale().x;
	}

private:
	float m_defaultRadius;
};

