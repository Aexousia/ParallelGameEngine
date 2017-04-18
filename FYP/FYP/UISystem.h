#pragma once
#include "imgui_impl_sdl_gl3.h"
#include <../dependancies/GL/glew.h>
#include "imgui.h"
#include "Framework.h"

class UISystem : public Singleton<UISystem>
{
public:
	void Render()
	{
		ImGui::Render();
	}

	void UpdateUI(float dt, SDL_Window* window)
	{
		ImGui_ImplSdlGL3_NewFrame(window);
		ImGui::ShowTestWindow();
	}

	void ProcessInput(SDL_Event& event)
	{
		ImGui_ImplSdlGL3_ProcessEvent(&event);
	}

private:

};
