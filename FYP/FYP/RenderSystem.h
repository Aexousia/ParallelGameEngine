#pragma once
#include "Framework.h"
#include "BasicTypes.h"
#include "UISystem.h"
#include "Camera\Camera.h"
#include <../dependancies/sdl/SDL_opengl.h>
#include "Shader.h"
#include "Assets.h"

class RenderSystem : public ISystem, public Singleton<RenderSystem>
{
public:
	void CameraInput(SDL_Event evt, float dt);

	void initialize(Size2D screenSize);

	void process(float dt);

private:
	//lights
	void bindLights(Shader* shader);

	void renderLights(glm::mat4& projection, glm::mat4& view, glm::mat4& VP);

	//models
	void renderModels(glm::mat4& projection, glm::mat4& view, glm::mat4& VP);

	//setup
	void setUpCamera();
	void setupWindow();
	void initGlew();

	//variables
	SDL_Window * m_window = nullptr;
	Size2D m_windowSize;
	SDL_GLContext m_glcontext;
	Camera m_camera;
};
