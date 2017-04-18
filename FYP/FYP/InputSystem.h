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
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_COMMA)
		{
			SINGLETON(TaskQueue)->incrementActiveWorkers();
			std::cout << "Active Workers" << SINGLETON(TaskQueue)->getNumActiveWorkers() << std::endl;
		}
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_PERIOD)
		{
			SINGLETON(TaskQueue)->decrementActiveWorkers();
			std::cout << "Active Workers" << SINGLETON(TaskQueue)->getNumActiveWorkers() << std::endl;
		}
	}
private:

};