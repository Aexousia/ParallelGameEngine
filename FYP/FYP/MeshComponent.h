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
		,	key(meshKey)
	{}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		MeshComponent* material = static_cast<MeshComponent*>(subject);
		if (change & GraphicsChanges::key)
		{
			key = material->key;
		}
	}

	inline void SetKey(const char* newKey)
	{
		key = newKey;
		SINGLETON(SyncManager)->registerChanges(this, GraphicsChanges::key);
	}

	const char* key;
};