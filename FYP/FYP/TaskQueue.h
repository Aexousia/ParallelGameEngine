#pragma once
#include <thread>
#include <queue>
#include <../dependancies/sdl/SDL.h>
#include <../dependancies/sdl/SDL_thread.h>
#include "Singleton.h"
#include <vector>
#include <functional>
#include <iostream>
#include "SyncManager.h"

// Thread Pool class allowing easy threading of tasks in c++
class TaskQueue : public Singleton<TaskQueue>
{
private:
	static TaskQueue * m_instance;
	SDL_mutex* m_queueLock, *m_threadIdLock, *m_processedLock, *m_activeWorkersLock;
	SDL_sem* m_canConsume, *m_workerSlotsFree, *m_idle;
	std::deque<std::function<void()>> m_jobs;
	std::vector<SDL_Thread*> m_workerPool;
	bool m_waitingUntilIdle;
	int m_busy;
	int m_numActiveWorkers;
	int m_maxActiveWorkers;
	std::vector<SDL_threadID> m_threadIDs;

public:
	bool threadingActive;
	void decrementActiveWorkers();
	void incrementActiveWorkers();
	int getNumActiveWorkers();
	bool isActive(int workerId);
	void registerThreadId();
	std::vector<SDL_threadID> getThreadIds();
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
	taskQueue->registerThreadId(); //this is done so that we don't have to synchronize access to vectors
	SDL_mutex * lock = taskQueue->getLock();
	SDL_sem * canConsume = taskQueue->canConsume();
	SDL_sem * workerSlotFree = taskQueue->workerSlotFree();
	while (true)
	{
		SDL_SemWait(workerSlotFree);
		SDL_SemWait(canConsume);
		auto& job = taskQueue->consumeJob();
		/*try
		{*/
			job();
			taskQueue->jobDone();
		/*}
		catch (...)
		{
			std::cout << "EARLY RETURN" << " worker: " << id << std::endl;
			taskQueue->jobDone();
		}*/
	}
}

#define TASK_BEGIN(...) SINGLETON(TaskQueue)->addJob(std::bind([__VA_ARGS__](){
#define TASK_END }));

//Helper macros
#define BATCH_LIST_BEGIN(vector, batchSize, elementName, ...) \
for (int i = 0, j = batchSize + 1; i < vector.size(); i += batchSize, j += batchSize)  \
{																				   \
	if (j > vector.size())														   \
	{																			   \
		j = vector.size();														   \
	}																			   \
	SINGLETON(TaskQueue)->addJob(std::bind([__VA_ARGS__](int min, int max) {			   \
		for (int i = min; i < max; i++)											   \
		{																		   \
			auto& elementName = vector[i];										   \



#define BATCH_LIST_END	\
		}				\
	}, i, j)); 			\
						\
}