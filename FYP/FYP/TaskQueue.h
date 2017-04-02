#pragma once
#include <thread>
#include <queue>
#include <../dependancies/sdl/SDL.h>
#include <../dependancies/sdl/SDL_thread.h>
#include "Singleton.h"
#include <vector>
#include <functional>
#include <iostream>

class TaskQueue : public Singleton<TaskQueue>
{
private:
	static TaskQueue * m_instance;
	SDL_mutex* m_queueLock, *m_processedLock, *m_activeWorkersLock;
	SDL_sem* m_canConsume, *m_workerSlotsFree, *m_idle;
	std::deque<std::function<void()>> m_jobs;
	std::vector<SDL_Thread*> m_workerPool;
	int m_busy;
	int m_numActiveWorkers;
	int m_maxActiveWorkers;

public:
	bool threadingActive;
	void decrementActiveWorkers();
	void incrementActiveWorkers();
	int getNumActiveWorkers();
	bool isActive(int workerId);
	TaskQueue();
	SDL_mutex * getLock();
	void spawnWorkers(unsigned int n=std::thread::hardware_concurrency());
	SDL_sem * canConsume();
	SDL_sem * workerSlotFree();
	std::function<void()> consumeJob();
	void waitUntilIdle();
	void jobDone();

	//*TaskQueue::addJob(std::bind(&Some_Class::Some_Method, &Some_object, ...Args)); 
	void addJob(std::function<void()> f);
};

static int worker(void* ptr)
{
	int* p_id = (int*)(ptr);
	int id = *p_id;
	delete p_id;
	srand(0);
	TaskQueue * taskQueue = SINGLETON(TaskQueue);
	SDL_mutex * lock = taskQueue->getLock();
	SDL_sem * canConsume = taskQueue->canConsume();
	SDL_sem * workerSlotFree = taskQueue->workerSlotFree();
	while (true)
	{
		SDL_SemWait(canConsume);
		SDL_SemWait(workerSlotFree);
		auto& job = taskQueue->consumeJob();
		try
		{
			job();
			taskQueue->jobDone();
		}
		catch (...)
		{
			std::cout << "EARLY RETURN" << " worker: " << id << std::endl;
		}
	}
}