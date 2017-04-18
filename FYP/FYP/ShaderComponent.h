#pragma once
#include <Framework.h>
#include <RenderSystem.h>
#include <GraphicsChanges.h>

struct ShaderComponent : public IComponent, public AutoMapper<ShaderComponent, RenderSystem>
{
	ShaderComponent(IEntity* parent, const char* shaderKey="default")
		:	SYSTEMS({
				SYSTEM(RenderSystem)
			})
		,	IComponent(parent)
		,	key(shaderKey)
	{}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		ShaderComponent* material = static_cast<ShaderComponent*>(subject);
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