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
		,	material(SINGLETON(AssetLoader)->findAssetByKey<Material>(materialKey))
	{}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		MaterialComponent* mat = static_cast<MaterialComponent*>(subject);
		if (change & GraphicsChanges::key)
		{
			material = mat->material;
		}
	}

	inline void SetKey(const char* newKey)
	{
		material = SINGLETON(AssetLoader)->findAssetByKey<Material>(newKey);
		SINGLETON(SyncManager)->registerChanges(this, GraphicsChanges::key);
	}

	Material* material;
};