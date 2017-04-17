#pragma once
#include "AssetLoader.h"
#include "MeshAsset.h"
#include "ShaderAsset.h"
#include "Light.h"
#include "Material.h"

inline void AddShadersToLoadQueue()
{
	std::vector<std::vector<const char*>> argLists = 
	{//	key, vertexShaderPath, fragmentShaderPath
		{ "default", "Assets/shader/phongShader.vert", "Assets/shader/phongShader.frag" },
		{"phong", "Assets/shader/phongShader.vert", "Assets/shader/phongShader.frag"},
	};

	for (auto& args : argLists)
	{
		SINGLETON(AssetLoader)->addAssetToLoadQueue(new ShaderAsset(args[0], args[1], args[2]));
	}
}

inline void CreateMaterials()
{
	SINGLETON(AssetLoader)->addAssetToMap<Material>("default",
		new Material(
			glm::vec3(0.2f, 0.2f, 0.2f), //Ambient reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Diffuse reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Specular reflectivity
			0.8f)						 //Specular shininess factor
		);

	SINGLETON(AssetLoader)->addAssetToMap<Material>("light",
		new Material(
			glm::vec3(1.f, 1.f, 1.f), //Ambient reflectivity
			glm::vec3(1.f, 1.f, 1.f), //Diffuse reflectivity
			glm::vec3(1.f, 1.f, 1.f), //Specular reflectivity
			1.f)					  //Specular shininess factor
		);
}

inline void AddMeshesToLoadQueue()
{
	std::vector<std::vector<const char*>> argLists =
	{//	key, objModelPath
		{ "sphere", "Assets/mesh/sphere.obj"},
		{ "cube", "Assets/mesh/cube.obj" },
		{ "light", "Assets/mesh/cube.obj" } 
	};

	for (auto& args : argLists)
	{
		SINGLETON(AssetLoader)->addAssetToLoadQueue(new MeshAsset(args[0], args[1]));
	}
}