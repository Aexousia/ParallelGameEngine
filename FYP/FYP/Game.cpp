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
#include <DemoScene4.h>
#include <DemoScene3.h>
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
	SINGLETON(UISystem)->initialize(&quit);

	//registerSystems
	REGISTER_SYSTEM(RenderSystem);
	REGISTER_SYSTEM_TICK_RATE(CollisionSystem, 15);
	REGISTER_SYSTEM(PhysicsSystem);

	//create scenes
	DemoScene1* demo1 = new DemoScene1();
	DemoScene2* demo2 = new DemoScene2();
	DemoScene3* demo3 = new DemoScene3();
	DemoScene4* demo4 = new DemoScene4();

	//add scenes
	SINGLETON(SceneManager)->addScene(demo1);
	SINGLETON(SceneManager)->addScene(demo2);
	SINGLETON(SceneManager)->addScene(demo3);
	SINGLETON(SceneManager)->addScene(demo4);

	//switch to initial scene
	SINGLETON(SceneManager)->setNextScene("default");

	return true;
}

void Game::destroy()
{

}

//** calls update on all game entities*/
void Game::update(float dt)
{
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

	float deltaTime = 0;
	
	while (!quit) { //game loop

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

		capTimer.start();
		unsigned int currentTime = LTimer::gameTime();//millis since game started
		deltaTime = (currentTime - lastTime) / 1000.0;//time since last update
		lastTime = currentTime; //save the curent time for next frame

		update(deltaTime);
		if (SINGLETON(SceneManager)->update()) //new scene is active, notify dependant systems
		{
			SINGLETON(CollisionSystem)->newScene();
			SINGLETON(SyncManager)->DistributeChanges();
		}

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}


