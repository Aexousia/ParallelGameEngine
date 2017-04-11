#pragma once
#include "Framework.h"
#include "BasicTypes.h"

class TestSystem : public ISystem, public IAutoMapUser<TestSystem>
{
public:
	TestSystem()
	{

	}

	void initialize(Size2D screenSize)
	{
		m_windowSize = screenSize;
	}

	void process(float dt);

private:
	Size2D m_windowSize;
};