#pragma once
#include <Framework.h>
#include <Sphere.h>
#include <Light.h>
#include <Cube.h>
#include <STDHelperFunctions.h>

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
		
		const int NUM_MATERIALS = 4;
		const char* materials[NUM_MATERIALS] = { "red", "blue", "green", "default" };
		for (int i = 0; i < 10; i++)
		{
			spheres.push_back(new Sphere(glm::vec3(), rand() % 12 + 2, materials[rand() % NUM_MATERIALS]));
		};

		std::vector<Light*> lights = {
			new Light(glm::vec3(20, 20, 0), Colour(), Colour(), Colour())
		};
		std::vector<Cube*> cubes = {
			new Cube(glm::vec3(), 1)
		};
		
		for (auto& sphere : spheres)
		{
			glm::vec3 velocity = randomUnitVector() * (rand() % 20 + 4.f);
			sphere->AddComponent<VelocityComponent>(new VelocityComponent(sphere,  velocity));
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
