#pragma once
#include <Framework.h>
#include <../dependancies/glm/vec3.hpp>
#include <VelocityComponent.h>
#include <TransformComponent.h>
#include <MeshComponent.h>
#include <ShaderComponent.h>
#include <MaterialComponent.h>
#include <BasicTypes.h>
#include <CollisionComponent.h>

class Sphere : public IEntity
{
public:
	Sphere(glm::vec3& position, float radius, 
		const char* material="default", const char* shader="default")
	{
		AddComponent<MeshComponent>(new MeshComponent(this, "sphere"));
		AddComponent<TransformComponent>(new TransformComponent(this, position, glm::vec3(), radius));
		AddComponent<MaterialComponent>(new MaterialComponent(this, material));
		AddComponent<ShaderComponent>(new ShaderComponent(this, shader));
		AddComponent<SphereCollisionComponent>(new SphereCollisionComponent(this));
	}

private:

};
