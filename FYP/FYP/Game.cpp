#include "stdafx.h"

#include "Game.h"
#include "LTimer.h"
#include <iostream>
#include <sstream>
using namespace std;

const int MAX_FPS = std::numeric_limits<int>::max();
const int SCREEN_TICKS_PER_FRAME = 1000 / MAX_FPS;

Game::Game(Size2D screenSize, Size2D worldSize) : 
	m_camera(new Camera2D(Rect(0, 0, screenSize.w, screenSize.h), 1)), 
	m_screenSize(screenSize), 
	m_worldSize(worldSize),
	m_threadingEnabled(true)
{
	TaskQueue::getInstance()->spawnWorkers();
	quit = false;
	m_camera->setLevelSize(Size2D(worldSize.w, worldSize.h));
}

Game::~Game()
{
}

bool Game::init() {
	Size2D winSize(m_screenSize.w, m_screenSize.h);
	srand(0);

	//creates our renderer, which looks after drawing and the window
	renderer.init(winSize, "Demo", m_camera);
	
	return true;
}

void Game::destroy()
{

}

//** calls update on all game entities*/
void Game::update(float dt)
{
	renderer.present();
}

/** update and render game entities*/
void Game::loop()
{
	LTimer capTimer;//to cap framerate

	int frameNum = 0;
	float timeSinceLastFrameCheck = 0;
	while (!quit) { //game loop

		capTimer.start();

		unsigned int currentTime = LTimer::gameTime();//millis since game started
		float deltaTime = (currentTime - lastTime) / 1000.0;//time since last update
		lastTime = currentTime; //save the curent time for next frame

		//inputManager.ProcessInput();

		update(deltaTime);

		if (m_threadingEnabled)
		{
			TaskQueue::getInstance()->waitUntilIdle();
		}
		else
		{

		}

		timeSinceLastFrameCheck += deltaTime;

		frameNum++;
		if (timeSinceLastFrameCheck > 1)
		{
			timeSinceLastFrameCheck = 0;

			//The frame rate as a string
			std::stringstream caption;

			//Calculate the frames per second and create the string
			caption << "Frames Per Second: " << frameNum;
			frameNum = 0;

			//Reset the caption
			SDL_SetWindowTitle(renderer.getWindow(), caption.str().c_str());
		}

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}


