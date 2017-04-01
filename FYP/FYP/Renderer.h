#pragma once
#define SDL_MAIN_HANDLED
#ifdef __APPLE__
#include "SDL2/SDL.h"
#elif defined(_WIN64) || defined(_WIN32)
#include "../dependancies/sdl/SDL.h"
#endif

#include "Camera2D.h"
#include "BasicTypes.h"


//Responsible for all drawing operations
//abstracts away specfic SDL specific drawing functions
class Renderer{

	// size of window in pixels
	Size2D windowSize;
	//position of window in world coordinates
	//change these if you want to zoom or pan
	Point2D viewportBottomLeft;
	Size2D viewportSize;


	SDL_Window *window;
	SDL_Renderer *sdl_renderer;
	Camera2D * m_camera;

public:
	Renderer();
	bool init(const Size2D&, const char*, Camera2D* cam);
	void setNewCamera(Camera2D * newCam);
	void drawRectOutline(const Rect & r, const Colour & c);
	void drawCircle(const Point2D & centre, const float & radius, const Colour& c);
	void drawRect(const Rect&, const Colour&);
	void present();
	void clear(const Colour&);
	Rect cameraTransform(Rect r);
	Point2D cameraTransform(Point2D p);
	float cameraTransform(float f);
	SDL_Window* getWindow()
	{
		return window;
	}

	void destroy();
	~Renderer();
};

