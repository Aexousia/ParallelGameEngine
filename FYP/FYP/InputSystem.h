#pragma once
#include "Framework.h"

class InputSystem : public Singleton<InputSystem>, public ISystem
{
public:
	void process(float dt) override
	{
	}

	void ProcessInput(SDL_Event& event)
	{
	}
private:

};