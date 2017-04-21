#pragma once
#include <Framework.h>
#include <RenderSystem.h>
#include <GraphicsChanges.h>

struct MeshComponent : public IComponent, public AutoMapper<MeshComponent, RenderSystem>
{
	MeshComponent(IEntity* parent, const char* meshKey="default")
		:	SYSTEMS({
				SYSTEM(RenderSystem)
			})
		,	IComponent(parent)
		,	mesh(SINGLETON(AssetLoader)->findAssetByKey<Mesh>(meshKey))
	{}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(subject);
		if (change & GraphicsChanges::key)
		{
			mesh = meshComponent->mesh;
		}
	}

	inline void SetKey(const char* newKey)
	{
		mesh = SINGLETON(AssetLoader)->findAssetByKey<Mesh>(newKey);
		SINGLETON(SyncManager)->registerChanges(this, GraphicsChanges::key);
	}

	Mesh* mesh;
};