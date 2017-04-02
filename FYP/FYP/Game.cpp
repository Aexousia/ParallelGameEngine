#include "stdafx.h"

#include "Game.h"
#include "LTimer.h"
#include <iostream>
#include <sstream>
#include "imgui_impl_sdl_gl3.h"
#include <../dependancies/GL/glew.h>
#include "imgui.h"
using namespace std;

const int MAX_FPS = std::numeric_limits<int>::max();
const int SCREEN_TICKS_PER_FRAME = 1000 / MAX_FPS;

Game::Game(Size2D screenSize, Size2D worldSize) : 
	m_screenSize(screenSize), 
	m_worldSize(worldSize),
	quit(false)
{
	SINGLETON(TaskQueue)->spawnWorkers();
	SINGLETON(TaskQueue)->threadingActive = true;
}

Game::~Game()
{
}

SDL_Window *window = nullptr;
bool Game::init() {
	Size2D winSize(m_screenSize.w, m_screenSize.h);
	srand(0);

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	window = SDL_CreateWindow("Parallel Game Engine - George Dixon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenSize.w, m_screenSize.h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_FULLSCREEN*/);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	glewInit();

	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(window);

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	return true;
}

void Game::destroy()
{

}

//** calls update on all game entities*/
void Game::update(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(window);

	for (int i = 0; i < 50; i++)
	{
		SINGLETON(TaskQueue)->addJob(
			std::bind([&] { float x;
		for (int y = 0; y < 10000; y++) { x = cosf(1782.45678) * cosf(2179271) * y; } })
		);
	}

	// Rendering
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	SDL_GL_SwapWindow(window);
}

/** update and render game entities*/
void Game::loop()
{
	LTimer capTimer;//to cap framerate
	SDL_Event event;

	int frameNum = 0;
	float timeSinceLastFrameCheck = 0;
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

		SINGLETON(TaskQueue)->waitUntilIdle();

		int frameTicks = capTimer.getTicks();//time since start of frame
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time before going to next frame
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}


