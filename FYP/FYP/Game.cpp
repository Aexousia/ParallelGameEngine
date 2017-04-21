#include "stdafx.h"
#include "imgui_impl_sdl_gl3.h"
#include "Game.h"
#include "LTimer.h"
#include <iostream>
#include <sstream>
#include "RenderSystem.h"
#include "Assets.h"
#include <DemoScene1.h>
#include <DemoScene2.h>
#include <CollisionSystem.h>
#include <PhysicsSystem.h>
#include <InputSystem.h>

using namespace std;

const int MAX_FPS = std::numeric_limits<int>::max();
const int SCREEN_TICKS_PER_FRAME = 1000 / MAX_FPS;

Game::Game(Size2D screenSize) : 
	m_screenSize(screenSize), 
	quit(false)
{
	SINGLETON(TaskQueue)->spawnWorkers();
	SINGLETON(TaskQueue)->threadingActive = true;
}

Game::~Game()
{
	SINGLETON(TaskQueue)->waitUntilIdle();
}

bool Game::init() {
	srand(time(NULL));

	//load assets into assetLoader
	CreateMaterials();
	SINGLETON(AssetLoader)->loadAll();

	//initialize systems
	SINGLETON(RenderSystem)->initialize(m_screenSize);
	SINGLETON(TestSystem)->initialize(m_screenSize);

	//registerSystems
	REGISTER_SYSTEM(RenderSystem);
	REGISTER_SYSTEM(CollisionSystem, 50);
	REGISTER_SYSTEM(PhysicsSystem, 100);

	//create scenes
	DemoScene1* demo1 = new DemoScene1();
	DemoScene2* demo2 = new DemoScene2();

	//add scenes
	SINGLETON(SceneManager)->addScene(demo1);
	SINGLETON(SceneManager)->addScene(demo2);

	//switch to initial scene
	SINGLETON(SceneManager)->setNextScene("DemoScene1");

	return true;
}

void Game::destroy()
{

}

//** calls update on all game entities*/
void Game::update(float dt)
{
	/*for (int i = 0; i < 50; i++)
	{
		TASK_BEGIN()
		{
			float x;
			for (int y = 0; y < 10000; y++) { x = cosf(1782.45678) * cosf(2179271) * y; }
		}
		TASK_END
		
	}*/

	SINGLETON(SystemManager)->runSystems(dt);
	SINGLETON(TaskQueue)->waitUntilIdle();
	SINGLETON(SyncManager)->DistributeChanges();
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
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
			SINGLETON(InputSystem)->ProcessInput(event);
			SINGLETON(UISystem)->ProcessInput(event);
			SINGLETON(RenderSystem)->CameraInput(event, deltaTime);
		}

		update(deltaTime);
		SINGLETON(SceneManager)->update();

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}


