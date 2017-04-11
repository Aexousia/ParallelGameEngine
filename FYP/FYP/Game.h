#pragma once

#include "BasicTypes.h"
#include <vector>
#include "Framework.h"
#include "../dependancies/sdl/SDL.h"
#include "Circle.h"

/** The game objct whic manages the game loop*/
class Game
{
	unsigned int lastTime;//time of last update;
	bool quit;

	Size2D m_screenSize;
	std::vector<Circle*> circles;

public:
	Game(Size2D screenSize);
	~Game();

	bool init();
	void destroy();

	void update(float dt);
	void loop();
};

