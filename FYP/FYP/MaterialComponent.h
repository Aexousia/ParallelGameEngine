#pragma once
#include <Framework.h>
#include <RenderSystem.h>
#include <GraphicsChanges.h>

struct MaterialComponent : public IComponent, public AutoMapper<MaterialComponent, RenderSystem>
{
	MaterialComponent(IEntity* parent, const char* materialKey="default")
		:	SYSTEMS({
				SYSTEM(RenderSystem)
			})
		,	IComponent(parent)
		,	key(materialKey)
	{}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		MaterialComponent* material = static_cast<MaterialComponent*>(subject);
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