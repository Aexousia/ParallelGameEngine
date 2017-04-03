#pragma once

#include "BasicTypes.h"
#include <vector>
#include "TaskQueue.h"
#include "../dependancies/sdl/SDL.h"
#include "AutoMapper.h"

/** The game objct whic manages the game loop*/
class Game
{
	unsigned int lastTime;//time of last update;
	bool quit;

	Size2D m_screenSize;
	Size2D m_worldSize;

public:
	Game(Size2D screenSize, Size2D worldSize);
	~Game();

	bool init();
	void destroy();

	void update(float dt);
	void loop();
};

