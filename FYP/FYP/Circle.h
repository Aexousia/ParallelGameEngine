#pragma once
#include "RenderSystem.h"
#include "Framework.h"
#include "TestSystem.h"
#include "BasicTypes.h"

namespace CircleChanges
{
	static const Change position = 1 << 1;
	static const Change radius = 1 << 2;
	static const Change direction = 1 << 3;
	static const Change all = position | radius | direction;
}

struct Circle : public IComponent, public AutoMapper<Circle, TestSystem, RenderSystem>
{
	Circle(Point2D pos, float rad) : 
		SYSTEMS({
			SYSTEM(RenderSystem),
			SYSTEM(TestSystem)
		}),
		position(pos),
		radius(rad),
		direction(1, 0)
	{

	}

	Circle() :
		position(Point2D(rand() % 1280, rand() % 720)),
		radius(rand() % 50 + 20),
		direction(Point2D(0, 1)),
		SYSTEMS({
			SYSTEM(RenderSystem),
			SYSTEM(TestSystem)
		})
	{

	}

	void ChangeOccured(Change change, IComponent* subject) override
	{
		Circle* circle = static_cast<Circle*>(subject);
		if (change & CircleChanges::position)
		{
			position = circle->position;
		}

		if (change & CircleChanges::radius)
		{
			radius = circle->radius;
		}

		if (change & CircleChanges::direction)
		{
			direction = circle->direction;
		}
	}

	void Move(Point2D mv)
	{
		position = position + mv;
		SINGLETON(SyncManager)->registerChanges(this, CircleChanges::position);
	}

	void SetRadius(float r)
	{
		radius = r;
		SINGLETON(SyncManager)->registerChanges(this, CircleChanges::radius);
	}

	void SetDirection(Point2D dir)
	{
		direction = dir;
		SINGLETON(SyncManager)->registerChanges(this, CircleChanges::direction);
	}

	Point2D position, direction;
	float radius;
};