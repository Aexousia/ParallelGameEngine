#include "stdafx.h"
#include "imgui_impl_sdl_gl3.h"
#include "Game.h"
#include "LTimer.h"
#include <iostream>
#include <sstream>
#include "RenderSystem.h"

using namespace std;

const int MAX_FPS = std::numeric_limits<int>::max();
const int SCREEN_TICKS_PER_FRAME = 1000 / MAX_FPS;

class Y : public ISystem, public IAutoMapUser<Y>
{
	void process(float dt) override {};
};

class Z : public ISystem, public IAutoMapUser<Y>
{
	void process(float dt) override {};
};

class X : public IComponent, public AutoMapper<X, Y, Z>
{
	char x[10000];
public:
	X() :
		SYSTEMS({
			SYSTEM(Y),
			SYSTEM(Z, Priority::High)
		}
		) {};
};

Game::Game(Size2D screenSize) : 
	m_screenSize(screenSize), 
	quit(false)
{
	SINGLETON(TaskQueue)->spawnWorkers();
	SINGLETON(TaskQueue)->threadingActive = true;
}

Game::~Game()
{
}

bool Game::init() {
	srand(time(NULL));

	//initialize systems
	SINGLETON(RenderSystem)->initialize(m_screenSize);

	//registerSystems
	REGISTER_SYSTEM_TICK_RATE(RenderSystem, 30);

	return true;
}

void Game::destroy()
{

}

//** calls update on all game entities*/
void Game::update(float dt)
{
	for (int i = 0; i < 50; i++)
	{
		SINGLETON(TaskQueue)->addJob(
			std::bind([&] { float x;
		for (int y = 0; y < 10000; y++) { x = cosf(1782.45678) * cosf(2179271) * y; } })
		);
	}

	SINGLETON(SystemManager)->runSystems(dt);
	SINGLETON(SyncManager)->DistributeChanges();
	SINGLETON(TaskQueue)->waitUntilIdle();
}

/** update and render game entities*/
void Game::loop()
{
	LTimer capTimer;//to cap framerate
	SDL_Event event;

	int frameNum = 0;
	float timeSinceLastFrameCheck = 0;

	SINGLETON(SyncManager)->DistributeChanges();

	while (!quit) { //game loop

		capTimer.start();
		unsigned int currentTime = LTimer::gameTime();//millis since game started
		float deltaTime = (currentTime - lastTime) / 1000.0;//time since last update
		lastTime = currentTime; //save the curent time for next frame

		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				quit = true;
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				quit = true;
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP)
			{
				SINGLETON(TaskQueue)->incrementActiveWorkers();
				std::cout << "Active Workers" << SINGLETON(TaskQueue)->getNumActiveWorkers() << std::endl;
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN)
			{
				SINGLETON(TaskQueue)->decrementActiveWorkers();
				std::cout << "Active Workers" << SINGLETON(TaskQueue)->getNumActiveWorkers() << std::endl;
			}
		}

		update(deltaTime);

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}


