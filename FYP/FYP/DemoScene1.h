#pragma once
#include <Framework.h>
#include <Sphere.h>
#include <Light.h>
#include <Cube.h>

class DemoScene1 : public IScene
{
public:
	DemoScene1() 
		:	IScene("DemoScene1")
	{
	}

	void enter() override
	{
		std::vector<Sphere*> spheres;

		for (int i = 0; i < 1; i++)
		{
			spheres.push_back(new Sphere(glm::vec3(), 100));
		}

		std::vector<Light*> lights = {
			new Light(glm::vec3(200, 200, 0), Colour(), Colour(), Colour())
		};
		std::vector<Cube*> cubes = {
			new Cube(glm::vec3(), 1000)
		};
		
		for (auto& sphere : spheres)
		{
			sphere->AddComponent<VelocityComponent>(new VelocityComponent(sphere, glm::vec3(100, 0, 0)));
			addEntity(sphere);
		}
		for (auto& light : lights)
		{
			addEntity(light);
		}
		for (auto& cube : cubes)
		{
			addEntity(cube);
		}
	}
};
