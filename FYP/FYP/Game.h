#pragma once

#include <vector>
#include "Renderer.h"
#include "Camera2D.h"
#include "TaskQueue.h"
#include "../dependancies/sdl/SDL.h"

/** The game objct whic manages the game loop*/
class Game
{
	Renderer renderer;

	unsigned int lastTime;//time of last update;

	bool pause;
	bool quit;

	Camera2D * m_camera;
	Size2D m_screenSize;
	Size2D m_worldSize;

	bool m_threadingEnabled;

public:
	Game(Size2D screenSize, Size2D worldSize);
	~Game();

	bool init();
	void destroy();

	void update(float dt);
	void loop();
};

