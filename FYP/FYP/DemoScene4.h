#pragma once
#include <Framework.h>
#include <Sphere.h>
#include <Light.h>
#include <Cube.h>
#include <STDHelperFunctions.h>
#include <MassComponent.h>

class DemoScene4 : public IScene
{
public:
	DemoScene4()
		: IScene("DemoScene4")
	{
	}

	void enter() override
	{
		std::vector<const char*> materials = { "emerald", "jade", "obsidian", "pearl", "ruby", "brass", "turquoise", "bronze", "chrome", "copper", "gold", "silver", "black plastic", "cyan plastic", "green plastic" };
		IEntity* monkey = new IEntity();

		monkey->AddComponent<MeshComponent>(new MeshComponent(monkey, "monkey"));
		monkey->AddComponent<TransformComponent>(new TransformComponent(monkey, glm::vec3(), glm::vec3(), 500));
		monkey->AddComponent<MaterialComponent>(new MaterialComponent(monkey, "ruby"));

		addEntity(monkey);

		std::vector<Light*> lights = {
			new Light(glm::vec3(20, 20, 0), Colour(), Colour(), Colour())
		};
		std::vector<Cube*> cubes = {
			new Cube(glm::vec3(), 1000)
		};
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
