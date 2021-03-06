#include "stdafx.h"
#include "RenderSystem.h"
#include "TransformComponent.h"
#include <MeshComponent.h>
#include <MaterialComponent.h>
#include <ShaderComponent.h>
#include <LightComponent.h>

RenderSystem::RenderSystem() : m_cameraSpin(0), m_cameraSpinSpeed(0.1f), m_cameraOffset(900)
{
}

void RenderSystem::CameraInput(SDL_Event evt, float dt)
{
	if (!SINGLETON(UISystem)->isCameraScripted())
	{
		static std::unordered_map<SDL_Keycode, bool> wasPressed;
		SDL_Keycode key = evt.key.keysym.sym;

		if (evt.type == SDL_KEYDOWN)
		{
			switch (key)
			{
			case (SDLK_w):
				m_camera.Move(FORWARD);
				break;
			case (SDLK_a):
				m_camera.Move(LEFT);
				break;
			case (SDLK_s):
				m_camera.Move(BACK);
				break;
			case (SDLK_d):
				m_camera.Move(RIGHT);
				break;
			case (SDLK_q):
				m_camera.Move(DOWN);
				break;
			case (SDLK_e):
				m_camera.Move(UP);
				break;
			case (SDLK_UP):
				m_camera.ChangePitch(0.08f);
				break;
			case (SDLK_DOWN):
				m_camera.ChangePitch(-0.08f);
				break;
			case (SDLK_LEFT):
				m_camera.ChangeHeading(0.08f);
				break;
			case (SDLK_RIGHT):
				m_camera.ChangeHeading(-0.08f);
				break;
			default:
				break;
			}
		}


		if (evt.type == SDL_KEYDOWN)
		{
			wasPressed[evt.key.keysym.sym] = true;
		}
		if (evt.type == SDL_KEYUP)
		{
			wasPressed[evt.key.keysym.sym] = false;
		}
	}
}

void RenderSystem::initialize(Size2D screenSize)
{
	m_windowSize = screenSize;

	setUpCamera();
	setupWindow();
	initGlew();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_STENCIL_TEST);

	AddMeshesToLoadQueue();
	AddShadersToLoadQueue();
	SINGLETON(AssetLoader)->loadAll();

	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	SDL_GL_MakeCurrent(m_window, NULL); //remove context binding of opengl from main thread
}

void RenderSystem::setUpCamera()
{
	m_camera.SetMode(CameraType::FREE);
	m_camera.SetPosition(glm::vec3(m_cameraOffset, m_cameraOffset, m_cameraOffset));
	m_camera.SetLookAt(glm::vec3(0, 0, 0));
	m_camera.SetWorldSize(m_windowSize.w, m_windowSize.h);
	m_camera.SetClipping(10, 2100);
	m_camera.SetFOV(45);
	m_camera.SetViewport(0, 0, m_windowSize.w, m_windowSize.h);
}

void RenderSystem::setupWindow()
{
	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	m_window = SDL_CreateWindow("Parallel Game Engine - George Dixon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowSize.w, m_windowSize.h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_FULLSCREEN*/);
	m_glcontext = SDL_GL_CreateContext(m_window);
}

void RenderSystem::initGlew()
{
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
}

void RenderSystem::process(float dt)
{
	//add binding of opengl context to this thread
	SDL_GL_MakeCurrent(m_window, m_glcontext);
	SINGLETON(UISystem)->UpdateUI(dt, m_window);

	if (SINGLETON(UISystem)->isCameraScripted())
	{
		m_cameraSpin -= dt * m_cameraSpinSpeed;
		if (m_cameraSpin < 0)
		{
			m_cameraSpin = 2 * PI;
		}
		m_camera.SetPosition(glm::vec3(m_cameraOffset* cos(m_cameraSpin), m_cameraOffset, m_cameraOffset * sin(m_cameraSpin)));
		m_camera.SetLookAt(glm::vec3(0, 0, 0));
	}

	//// Rendering
	glViewport(0, 0, (int)m_windowSize.w, (int)m_windowSize.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	m_camera.Update();

	glm::mat4 model, view, projection;
	m_camera.GetMatricies(projection, view, model);
	glm::mat4 VP = projection * view;	//vp matrix

	renderModelsInstanced(projection, view, VP);
	renderLights(projection, view, VP);

	////RenderUI last
	SINGLETON(UISystem)->Render();
	SDL_GL_SwapWindow(m_window);

	////remove context binding of opengl from this thread
	SDL_GL_MakeCurrent(m_window, NULL);
}

void RenderSystem::renderModels(glm::mat4& projection, glm::mat4& view, glm::mat4& VP)
{
	auto models = AutoMap::getComponentGroups<RenderSystem, MeshComponent,
		TransformComponent,
		MaterialComponent,
		ShaderComponent>();
	for (auto model : models)
	{
		MeshComponent* modelMesh;
		TransformComponent* modelTransform;
		MaterialComponent* modelMaterial;
		ShaderComponent* modelShader;
		NULL_COMPONENT* _;
		std::tie(modelMesh, modelTransform, modelMaterial, modelShader, _) = model;

		//get shader
		auto shader = SINGLETON(AssetLoader)->findAssetByKey<Shader>(modelShader->key);
		Shader::bind(shader); // bind shader
		bindLights(shader); // bind lights to shader

							//get and bind material to shader
		Material* mat = modelMaterial->material;
		mat->BindToShader(shader, "Material");

		//get matrices for model
		glm::mat4 MVP = modelTransform->GetMVP(VP);
		glm::mat4 Model = modelTransform->GetModel();
		glm::mat4 ModelView = view * Model;
		glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));

		// set matrices
		GLuint vars[3] = {
			shader->getUniformLocation("ModelViewMatrix"),
			shader->getUniformLocation("NormalMatrix"),
			shader->getUniformLocation("MVP")
		};

		glUniformMatrix4fv(vars[0], 1, GL_FALSE, glm::value_ptr(ModelView));
		glUniformMatrix4fv(vars[1], 1, GL_FALSE, glm::value_ptr(Normal));
		glUniformMatrix4fv(vars[2], 1, GL_FALSE, glm::value_ptr(MVP));

		//render model
		modelMesh->mesh->render();
	}
}

void RenderSystem::renderModelsInstanced(glm::mat4& projection, glm::mat4& view, glm::mat4& VP)
{
	auto models = AutoMap::getComponentGroups<RenderSystem,  MeshComponent, 
															 TransformComponent, 
															 MaterialComponent>();
	if (models.size())
	{
		Shader* lastShader = nullptr;
		Material* lastMat = nullptr;
		std::vector<GLint> vars;
		std::unordered_map<Mesh*, std::unordered_map<Material*, std::tuple<std::vector<glm::mat4>, std::vector<glm::mat4>, std::vector<glm::mat4>>>> instanceData;


		for (auto model : models)
		{
			MeshComponent* modelMesh;
			TransformComponent* modelTransform;
			MaterialComponent* modelMaterial;
			NULL_COMPONENT* _;
			std::tie(modelMesh, modelTransform, modelMaterial, _, _) = model;

			//get matrices for model
			glm::mat4 MVP = modelTransform->GetMVP(VP);
			glm::mat4 Model = modelTransform->GetModel();
			glm::mat4 ModelView = view * Model;
			glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));

			auto material = modelMaterial->material;
			auto mesh = modelMesh->mesh;
			std::get<0>(instanceData[mesh][material]).push_back(MVP);
			std::get<1>(instanceData[mesh][material]).push_back(ModelView);
			std::get<2>(instanceData[mesh][material]).push_back(Normal);
		}

		
		auto shader = SINGLETON(AssetLoader)->findAssetByKey<Shader>("phong");
		Shader::bind(shader); // bind shader
		bindLights(shader); // bind lights to shader
				
		for (auto& outerKv : instanceData)
		{
			auto mesh = outerKv.first;
			if (mesh == SINGLETON(AssetLoader)->findAssetByKey<Mesh>("cube"))
			{
				glCullFace(GL_FRONT);
			}
			for (auto& kv : outerKv.second)
			{
				auto material = kv.first;
				material->BindToShader(shader, "Material");

				auto& meshEntries = mesh->getMeshEntries();
				for (auto& me : meshEntries)
				{
					me->renderInstanced(std::get<0>(kv.second), std::get<1>(kv.second), std::get<2>(kv.second));
				}
			}
			if (mesh == SINGLETON(AssetLoader)->findAssetByKey<Mesh>("cube"))
			{
				glCullFace(GL_BACK);
			}
		}
	}
}

void RenderSystem::bindLights(Shader* shader)
{
	std::vector<LightComponent*> lightComponents = AutoMap::getList<LightComponent, RenderSystem>();

	for (int i = 0; i < lightComponents.size(); i++)
	{
		lightComponents[i]->BindToShader(shader, i, "Light");
	}

	GLint iLightCount = shader->getUniformLocation("LIGHTCOUNT");
	glUniform1i(iLightCount, lightComponents.size());
}

void RenderSystem::renderLights(glm::mat4& projection, glm::mat4& view, glm::mat4& VP)
{
	//std::vector<LightComponent*> lightComponents = AutoMap::getList<LightComponent, RenderSystem>();

	////get shader
	//auto shader = SINGLETON(AssetLoader)->findAssetByKey<Shader>("default");
	//Shader::bind(shader); // bind shader
	//bindLights(shader); // bind lights to shader

	////get and bind material to shader
	//Material* mat = SINGLETON(AssetLoader)->findAssetByKey<Material>("light");
	//mat->BindToShader(shader, "Material");

	//// get uniform locations
	//GLuint vars[3] = {
	//	shader->getUniformLocation("ModelViewMatrix"),
	//	shader->getUniformLocation("NormalMatrix"),
	//	shader->getUniformLocation("MVP")
	//};

	//auto lightModel = SINGLETON(AssetLoader)->findAssetByKey<Mesh>("cube");
	//TransformComponent t = TransformComponent(nullptr);

	//for (auto& light : lightComponents)
	//{
	//	t.SetPos(light->Position);
	//	glm::mat4 MVP = t.GetMVP(VP);
	//	glm::mat4 Model = t.GetModel();
	//	glm::mat4 ModelView = view * Model;
	//	glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));

	//	glUniformMatrix4fv(vars[0], 1, GL_FALSE, glm::value_ptr(ModelView));
	//	glUniformMatrix4fv(vars[1], 1, GL_FALSE, glm::value_ptr(Normal));
	//	glUniformMatrix4fv(vars[2], 1, GL_FALSE, glm::value_ptr(MVP));

	//	//render model
	//	lightModel->render();
	//}

	//Shader::unbind();
}
