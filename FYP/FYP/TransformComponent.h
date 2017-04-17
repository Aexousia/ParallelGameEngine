#pragma once

#include <Framework.h>
#include <RenderSystem.h>
#include <../dependancies/glm/glm.hpp>
#include <../dependancies/glm/gtx/transform.hpp>

namespace TransformChanges
{
	static const Change position = 1 << 1;
	static const Change rotation = 1 << 2;
	static const Change scale = 1 << 3;
	static const Change all = position | rotation | scale;
}

struct TransformComponent : public IComponent, public AutoMapper<TransformComponent, RenderSystem>
{
public:
	TransformComponent(IEntity* parent,
		const glm::vec3& pos = glm::vec3(), 
		const glm::vec3& rot = glm::vec3(), 
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
		:	pos(pos)
		,	rot(rot)
		,	scale(scale)
		,	IComponent(parent)
		,	SYSTEMS({
				SYSTEM(RenderSystem)
			})
	{
	}

	inline void ChangeOccured(Change change, IComponent* subject) override
	{
		TransformComponent* transform = static_cast<TransformComponent*>(subject);
		if (change & TransformChanges::position)
		{
			pos = transform->GetPos();
		}

		if (change & TransformChanges::rotation)
		{
			rot = transform->GetRot();
		}

		if (change & TransformChanges::scale)
		{
			scale = transform->GetScale();
		}
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMat = glm::translate(pos);
		glm::mat4 scaleMat = glm::scale(scale);
		glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;

		return posMat * rotMat * scaleMat;
	}

	inline glm::mat4 GetMVP(const glm::mat4& VP) const
	{
		glm::mat4 M = GetModel();

		return VP * M;//camera.GetViewProjection() * GetModel();
	}

	inline glm::vec3 GetPos() { return pos; }
	inline glm::vec3 GetRot() { return rot; }
	inline glm::vec3 GetScale() { return scale; }

	inline void SetPos(glm::vec3& pos) 
	{ 
		this->pos = pos; 
		SINGLETON(SyncManager)->registerChanges(this, TransformChanges::position);
	}

	inline void SetRot(glm::vec3& rot) 
	{
		this->rot = rot; 
		SINGLETON(SyncManager)->registerChanges(this, TransformChanges::rotation);
	}

	inline void SetScale(glm::vec3& scale) 
	{ 
		this->scale = scale;
		SINGLETON(SyncManager)->registerChanges(this, TransformChanges::scale);
	}

protected:
private:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};