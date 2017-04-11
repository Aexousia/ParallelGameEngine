#include "stdafx.h"
#include "TestSystem.h"
#include "Circle.h"

void TestSystem::process(float dt)
{
	auto circles = AutoMap::get<Circle, TestSystem>();

	float speed = 50;
	BATCH_LIST_BEGIN(circles, 20, circle, &, speed, dt,)
	{
		Point2D velocity = (circle->direction * speed);
		Point2D projectedPosition = circle->position + velocity;
		if (projectedPosition.x < 0 - circle->radius || projectedPosition.x > m_windowSize.w + circle->radius)
		{
			circle->SetDirection(circle->direction * Point2D(-1, 1));
			velocity = (circle->direction * speed);
		}
		if (projectedPosition.y < 0 - circle->radius || projectedPosition.y > m_windowSize.h + circle->radius)
		{
			circle->SetDirection(circle->direction * Point2D(1, -1));
			velocity = (circle->direction * speed);
		}
		circle->Move(velocity * dt);
	}
	BATCH_LIST_END
}