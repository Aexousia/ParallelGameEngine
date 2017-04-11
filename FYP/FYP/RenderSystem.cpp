#include "stdafx.h"
#include "RenderSystem.h"
#include "Circle.h"

void RenderSystem::process(float dt)
{
	//add binding of opengl context to this thread
	SDL_GL_MakeCurrent(m_window, m_glcontext);
	SINGLETON(UISystem)->UpdateUI(dt, m_window);

	// Rendering
	glViewport(0, 0, (int)m_windowSize.w, (int)m_windowSize.h);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_windowSize.w, m_windowSize.h, 0, 0.f, 1.f);//fov, aspect, zNear, zFar

	auto& circles = AutoMap::get<Circle, RenderSystem>();
	for (auto& circle : circles)
	{
		drawFilledCircle(circle->position.x, circle->position.y, circle->radius);
	}

	//RenderUI last
	SINGLETON(UISystem)->Render();
	SDL_GL_SwapWindow(m_window);

	//remove context binding of opengl from this thread
	SDL_GL_MakeCurrent(m_window, NULL);
}
