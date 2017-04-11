#pragma once
#include "Framework.h"
#include "BasicTypes.h"
#include "UISystem.h"

class RenderSystem : public ISystem, public Singleton<RenderSystem>
{
public:
	RenderSystem()
	{

	}

	void initialize(Size2D screenSize)
	{
		m_windowSize = screenSize;

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
		m_window = SDL_CreateWindow("Parallel Game Engine - George Dixon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowSize.w, m_windowSize.h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_FULLSCREEN*/);
		m_glcontext = SDL_GL_CreateContext(m_window);

		GLint GlewInitResult = glewInit();
		if (GLEW_OK != GlewInitResult)
		{
			printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
			exit(EXIT_FAILURE);
		}

		// Setup ImGui binding
		ImGui_ImplSdlGL3_Init(m_window);

		// Load Fonts
		// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
		//ImGuiIO& io = ImGui::GetIO();
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
		//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

		SDL_GL_MakeCurrent(m_window, NULL); //remove context binding of opengl from main thread
	}

	void process(float dt);

	void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
		int i;
		int triangleAmount = 20; //# of triangles used to draw circle

								 //GLfloat radius = 0.8f; //radius
		GLfloat twicePi = 2.0f * 3.14;

		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for (i = 0; i <= triangleAmount; i++) {
			glVertex2f(
				x + (radius * cos(i *  twicePi / triangleAmount)),
				y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
		glEnd();
	}

private:
	SDL_Window * m_window = nullptr;
	Size2D m_windowSize;
	SDL_GLContext m_glcontext;
};
