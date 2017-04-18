#pragma once
#include <Singleton.h>
#include <unordered_map>
#include <ECSInterfaces.h>

class IScene
{
public:
	IScene(std::string key)
		:	m_key(key)
	{}

	virtual void addEntity(IEntity* entity)
	{
		m_entities.push_back(entity);
	}

	virtual std::string getKey()
	{
		return m_key;
	}

	virtual void cleanupDeadEntities()
	{
		for (auto& entity : m_entities)
		{
			if (!entity->alive)
			{
				delete entity;
				entity = nullptr;
			}
		}

		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), nullptr), m_entities.end());
	}

	virtual void enter() = 0;

	virtual void exit()
	{
		for (auto& entity : m_entities)
		{
			delete entity;
		}
		m_entities.clear();
	}

protected:
	std::string  m_key;
	std::vector<IEntity*> m_entities;
};

class SceneManager : public Singleton<SceneManager>
{
public:
	//simple getter which tells you the current scene, may not be needed
	const std::string getCurrentScene()
	{
		return m_currentScene->getKey();
	}

	void update()
	{
		if (m_currentScene != nullptr)
		{
			if (m_currentScene->getKey() != m_nextScene)
			{
				switchTo(m_nextScene);
			}
			else
			{
				m_currentScene->cleanupDeadEntities();
			}
		}
		else
		{
			printf("No Scene Active \n");
		}
	}

	void setNextScene(std::string nextScene)
	{
		m_nextScene = nextScene;
		if (!m_currentScene) //if no scene is active, swap straight away
		{
			switchTo(m_nextScene);
		}
	}

	//adds a scene object to the scene map
	void addScene(IScene* newScene)
	{
		m_scenes[newScene->getKey()] = newScene;
	}

private:

	//switch to a scene by string name
	void switchTo(std::string scene)
	{
		bool sceneFound = false;
		IScene* nextScene = nullptr;

		//checks if the scene is there, if it isn't, do nothing
		auto& it = m_scenes.find(scene);
		if (it != m_scenes.end())
		{
			sceneFound = true;
			nextScene = it->second;
		}
		else
		{
			printf("scene not found");
			if (m_currentScene != nullptr)
			{
				m_nextScene = m_currentScene->getKey();
			}
			return;
		}

		if (sceneFound)
		{
			if (m_currentScene != nullptr)
			{
				//exit the old scene 
				m_currentScene->exit();
			}

			//enter the new scene
			m_currentScene = nextScene;
			m_nextScene = m_currentScene->getKey();
			m_currentScene->enter();
		}
	}

	IScene* m_currentScene;
	std::string m_nextScene;
	std::unordered_map<std::string, IScene*> m_scenes;
};