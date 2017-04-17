#pragma once
#include "AssetLoader.h"
#include "IOHelperFunctions.h"
#include "Mesh.h"

class MeshAsset : public Asset
{
public:
	MeshAsset(const char* key, const char* objPath)
	:	Asset(key)
	,	m_objFilePath(objPath)
	{
	}

	virtual bool load() override
	{
		assert(fileExists(m_objFilePath));
		SINGLETON(AssetLoader)->addAssetToMap<Mesh>(getKey(), new Mesh(m_objFilePath));
		return true;
	}
private:
	const char* m_objFilePath;
};