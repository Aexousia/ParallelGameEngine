#pragma once
#include "AssetLoader.h"
#include "MeshAsset.h"
#include "ShaderAsset.h"
#include "Material.h"

//called from renderSystem init, openglContext required
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

	SINGLETON(AssetLoader)->addAssetToMap<Material>("red",
		new Material(
			glm::vec3(1.f, 0, 0), //Ambient reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Diffuse reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Specular reflectivity
			1.f)					  //Specular shininess factor
		);

	SINGLETON(AssetLoader)->addAssetToMap<Material>("blue",
		new Material(
			glm::vec3(0, 1.f, 0), //Ambient reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Diffuse reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Specular reflectivity
			1.f)					  //Specular shininess factor
		);

	SINGLETON(AssetLoader)->addAssetToMap<Material>("green",
		new Material(
			glm::vec3(0, 0, 1.f), //Ambient reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Diffuse reflectivity
			glm::vec3(0.8f, 0.8f, 0.8f), //Specular reflectivity
			1.f)					  //Specular shininess factor
		);

	SINGLETON(AssetLoader)->addAssetToMap<Material>("emerald",
		new Material(
			glm::vec3(0.0215, 0.1745, 0.0215), //Ambient reflectivity
			glm::vec3(0.07568, 0.61424, 0.07568), //Diffuse reflectivity
			glm::vec3(0.633, 0.727811, 0.633), //Specular reflectivity
			0.6)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("jade",
		new Material(
			glm::vec3(0.135, 0.2225, 0.1575), //Ambient reflectivity
			glm::vec3(0.54, 0.89, 0.63), //Diffuse reflectivity
			glm::vec3(0.316228, 0.316228, 0.316228), //Specular reflectivity
			0.1)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("obsidian",
		new Material(
			glm::vec3(0.05375, 0.05, 0.06625), //Ambient reflectivity
			glm::vec3(0.18275, 0.17, 0.22525), //Diffuse reflectivity
			glm::vec3(0.332741, 0.328634, 0.346435), //Specular reflectivity
			0.3)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("pearl",
		new Material(
			glm::vec3(0.25, 0.20725, 0.20725), //Ambient reflectivity
			glm::vec3(1, 0.829, 0.829), //Diffuse reflectivity
			glm::vec3(0.296648, 0.296648, 0.296648), //Specular reflectivity
			0.088)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("ruby",
		new Material(
			glm::vec3(0.1745,	0.01175,	0.01175), //Ambient reflectivity
			glm::vec3(0.61424,	0.04136,	0.04136), //Diffuse reflectivity
			glm::vec3(0.727811,	0.626959,	0.626959), //Specular reflectivity
			0.6f)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("turquoise",
		new Material(
			glm::vec3(0.1,	0.18725,	0.1745), //Ambient reflectivity
			glm::vec3(0.396,	0.74151,	0.69102), //Diffuse reflectivity
			glm::vec3(0.297254,	0.30829,	0.306678), //Specular reflectivity
			0.1)					  //Specular shininess factor
		);

	SINGLETON(AssetLoader)->addAssetToMap<Material>("brass",
		new Material(
			glm::vec3(0.329412,	0.223529,	0.027451), //Ambient reflectivity
			glm::vec3(0.780392,	0.568627,	0.113725), //Diffuse reflectivity
			glm::vec3(0.992157,	0.941176,	0.807843), //Specular reflectivity
			0.21794872)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("bronze",
		new Material(
			glm::vec3(0.2125,	0.1275,	0.054), //Ambient reflectivity
			glm::vec3(0.714,	0.4284,	0.18144), //Diffuse reflectivity
			glm::vec3(0.393548,	0.271906,	0.166721), //Specular reflectivity
			0.2f)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("chrome",
		new Material(
			glm::vec3(0.25,	0.25,	0.25), //Ambient reflectivity
			glm::vec3(0.4,	0.4	,0.4), //Diffuse reflectivity
			glm::vec3(0.774597,	0.774597,	0.774597), //Specular reflectivity
			0.6f)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("copper",
		new Material(
			glm::vec3(0.19125,	0.0735,	0.0225), //Ambient reflectivity
			glm::vec3(0.7038,	0.27048,	0.0828), //Diffuse reflectivity
			glm::vec3(0.256777,	0.137622,	0.086014), //Specular reflectivity
			0.1f)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("gold",
		new Material(
			glm::vec3(0.24725,	0.1995,	0.0745), //Ambient reflectivity
			glm::vec3(0.75164,	0.60648,	0.22648), //Diffuse reflectivity
			glm::vec3(0.628281,	0.555802,	0.366065), //Specular reflectivity
			0.4)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("silver",
		new Material(
			glm::vec3(0.19225,	0.19225,	0.19225), //Ambient reflectivity
			glm::vec3(0.50754,	0.50754,	0.50754), //Diffuse reflectivity
			glm::vec3(0.508273,	0.508273,	0.508273), //Specular reflectivity
			0.4)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("black plastic",
		new Material(
			glm::vec3(0.0,	0.0,	0.0), //Ambient reflectivity
			glm::vec3(0.01,	0.01,	0.01), //Diffuse reflectivity
			glm::vec3(0.50,	0.50,	0.50), //Specular reflectivity
			0.25f)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("cyan plastic",
		new Material(
			glm::vec3(0.0,	0.1,	0.06), //Ambient reflectivity
			glm::vec3(0.0,	0.50980392,	0.50980392), //Diffuse reflectivity
			glm::vec3(0.50196078,	0.50196078,	0.50196078), //Specular reflectivity
			0.25f)					  //Specular shininess factor
		);
	SINGLETON(AssetLoader)->addAssetToMap<Material>("green plastic",
		new Material(
			glm::vec3(0.0,	0.0,	0.0), //Ambient reflectivity
			glm::vec3(0.1	,0.35,	0.1), //Diffuse reflectivity
			glm::vec3(0.45,	0.55,	0.45), //Specular reflectivity
			0.25f)					  //Specular shininess factor
		);
}

//called from renderSystem init, openglContext required
inline void AddMeshesToLoadQueue()
{
	std::vector<std::vector<const char*>> argLists =
	{//	key, objModelPath
		{ "sphere", "Assets/mesh/sphere.obj"},
		{ "cube", "Assets/mesh/cube.obj" },
		{ "monkey", "Assets/mesh/monkey.obj" },
		{ "default", "Assets/mesh/monkey.obj" } 
	};

	for (auto& args : argLists)
	{
		SINGLETON(AssetLoader)->addAssetToLoadQueue(new MeshAsset(args[0], args[1]));
	}
}