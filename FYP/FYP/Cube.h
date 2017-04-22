#pragma once
#include <Framework.h>
#include <../dependancies/glm/vec3.hpp>
#include <VelocityComponent.h>
#include <TransformComponent.h>
#include <MeshComponent.h>
#include <ShaderComponent.h>
#include <MaterialComponent.h>
#include <BasicTypes.h>

class Cube : public IEntity
{
public:
	Cube(glm::vec3& position, float heightWidthDepth,
		const char* material = "default", const char* shader = "default")
	{
		AddComponent<MeshComponent>(new MeshComponent(this, "cube"));
		AddComponent<TransformComponent>(new TransformComponent(this, position, glm::vec3(), heightWidthDepth / 2.f));
		AddComponent<MaterialComponent>(new MaterialComponent(this, material));
		AddComponent<ShaderComponent>(new ShaderComponent(this, shader));
		AddComponent<AxisAlignedCubeCollisionComponent>(new AxisAlignedCubeCollisionComponent(this));
	}

	virtual ~Cube() {}

private:

};
