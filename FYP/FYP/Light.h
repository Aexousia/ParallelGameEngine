#pragma once
#include <Framework.h>
#include <BasicTypes.h>
#include <LightComponent.h>

class Light : public IEntity
{
public:
	Light(glm::vec3& position, Colour& ambient, Colour& diffuse, Colour& specular)
	{
		float c = 255.0f; 
		AddComponent<LightComponent>(new LightComponent(
			position,													//Light Position in eye-coords
			glm::vec3(ambient.r / c, ambient.g / c, ambient.b / c),		//Ambient light intensity
			glm::vec3(diffuse.r / c, diffuse.g / c, diffuse.b / c),		//Diffuse light intensity
			glm::vec3(specular.r / c, specular.g / c, specular.b / c),	//Specular light intensity)
			this
		));
	}
};