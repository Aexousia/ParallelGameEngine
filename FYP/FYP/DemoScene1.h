#pragma once
#include <Framework.h>
#include <Sphere.h>
#include <Light.h>
#include <Cube.h>
#include <STDHelperFunctions.h>
#include <MassComponent.h>

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
		std::vector<const char*> materials = { "emerald", "jade", "obsidian", "pearl", "ruby", "brass", "turquoise", "bronze", "chrome", "copper", "gold", "silver", "black plastic", "cyan plastic", "green plastic" };
		for (int i = 0; i < 2000; i++)
		{
			int radius = rand() % 12 + 7;
			auto sphere = new Sphere(glm::vec3(), radius, materials[rand() % materials.size()]);
			sphere->AddComponent<MassComponent>(new MassComponent(sphere, 4.f/3.f * radius * radius * radius * PI));
			spheres.push_back(sphere);
		};

		std::vector<Light*> lights = {
			new Light(glm::vec3(20, 20, 0), Colour(), Colour(), Colour())
		};
		std::vector<Cube*> cubes = {
			new Cube(glm::vec3(), 1000)
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
