#pragma once
#include "imgui_impl_sdl_gl3.h"
#include <../dependancies/GL/glew.h>
#include "imgui.h"
#include "Framework.h"
#include <string>

class UISystem : public Singleton<UISystem>
{
public:
	UISystem()
		:	m_cameraScripted(true)
		,	m_exit(nullptr)
		,	m_collisionBatchSize(10)
		,	m_physicsBatchSize(100)
	{

	}

	int getCollisionBatchSize()
	{
		return m_collisionBatchSize;
	}

	int getPhysicsBatchSize()
	{
		return m_physicsBatchSize;
	}

	void initialize(bool* exit)
	{
		m_exit = exit;
	}

	void Render()
	{
		ImGui::Render();
	}

	bool isCameraScripted()
	{
		return m_cameraScripted;
	}

	void UpdateUI(float dt, SDL_Window* window)
	{
		ImGui_ImplSdlGL3_NewFrame(window);

		if (ImGui::CollapsingHeader("Granularity"))
		{
			ImGui::SliderInt("Collision System Batch Size", &m_collisionBatchSize, 10, 500);
			ImGui::SliderInt("Physics System Batch Size", &m_physicsBatchSize, 10, 500);
		}
		if (ImGui::CollapsingHeader("Stats"))
		{
			FPSGraph();
			ImGui::Separator();
			auto& cpuStats = SINGLETON(TaskQueue)->getCpuMetrics();
			for (int i = 0; i < cpuStats.size(); i++)
			{
				ImGui::ProgressBar(cpuStats[i] / 100.f, ImVec2(0.0f, 0.0f));
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
				std::string title = "CPU Load Core: ";
				title += std::to_string(i);
				ImGui::Text(title.c_str());
			}
		}	
		ImGui::Separator();
		ImGui::Text("Options");
		static int i1 = SINGLETON(TaskQueue)->getNumActiveWorkers();
		ImGui::SliderInt("Number Active Threads", &i1, 1, std::thread::hardware_concurrency());
		while (i1 != SINGLETON(TaskQueue)->getNumActiveWorkers())
		{
			if (i1 < SINGLETON(TaskQueue)->getNumActiveWorkers())
			{
				SINGLETON(TaskQueue)->decrementActiveWorkers();
			}
			else
			{
				SINGLETON(TaskQueue)->incrementActiveWorkers();
			}
		}
		ImGui::Separator();
		const char* scenes[] = { "default", "DemoScene4", "DemoScene3", "DemoScene2", "DemoScene1" };
		const char* aliases[] = { "default (empty)", "Monkey", "1000 Balls", "1500 Balls", "2000 Balls" };
		static int currentScene = 3;
		ImGui::ListBox("Scenes", &currentScene, aliases, 5, 4);
		ImGui::SameLine();
		if (ImGui::Button("Switch Scene \n(double click)"))
		{
			SINGLETON(SceneManager)->setNextScene(scenes[currentScene]);
		}
		ImGui::Separator();
		ImGui::Checkbox("ScriptedCamera", &m_cameraScripted);
		ImGui::Separator();
		if (ImGui::Button("Quit Application"))
		{
			(*m_exit) = true;
		}
	}

	void ProcessInput(SDL_Event& event)
	{
		ImGui_ImplSdlGL3_ProcessEvent(&event);
	}

private:

	void FPSGraph()
	{
		static std::vector<float> frames;
		frames.push_back(ImGui::GetIO().Framerate);
		if (frames.size() > 200)
		{
			assert(!frames.empty());
			frames.erase(frames.begin());
		}

		float data[100];
		int frameOffset = 0;
		if (frames.size() > 100)
		{
			frameOffset = frames.size() - 100;
		}
		int maxFps = -1;
		int fpsSum = 0;
		for (auto& frame : frames)
		{
			if (frame > maxFps)
			{
				maxFps = frame;
			}
			fpsSum += frame;
		}
		int fpsAvg = (frames.size() > 0) ? fpsSum / frames.size() : 0;
		for (int i = 0; i < 100, frameOffset + i < frames.size(); i++)
		{
			data[i] = frames[frameOffset + i];
		}
		std::string avg = "avg ";
		std::string title = "FPS: ";
		title += std::to_string(int(frames.back()));
		avg += std::to_string(fpsAvg);
		ImGui::PlotHistogram(title.c_str(), data, 100, 0, avg.c_str(), 0, maxFps * 1.1f, ImVec2(0, 80));
	}


	bool m_cameraScripted;
	bool* m_exit;

	int m_collisionBatchSize;
	int m_physicsBatchSize;
};
