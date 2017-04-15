#pragma once
#include "AssetLoader.h"
#include "MeshAsset.h"
#include "ShaderAsset.h"


inline void AddShadersToLoadQueue()
{
	std::vector<std::vector<const char*>> argLists = 
	{
		{ "sphere", "sphereShader.vert", "sphereShader.frag"},
	};

	for (auto& args : argLists)
	{
		SINGLETON(AssetLoader)->addAssetToLoadQueue(new ShaderAsset(args[0], args[1], args[2]));
	}
}