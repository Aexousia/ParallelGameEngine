#include "stdafx.h"
#include "TaskQueue.h"

TaskQueue * TaskQueue::m_instance = nullptr;

TaskQueue::TaskQueue() : m_canConsume(SDL_CreateSemaphore(0)), m_queueLock(SDL_CreateMutex()), m_processedLock(SDL_CreateMutex())
{
}

SDL_mutex * TaskQueue::getLock()
{
	return m_queueLock;
}

void TaskQueue::spawnWorkers(unsigned int numWorkers)
{
	for (unsigned int i = 0; i < numWorkers; i++)
	{
		m_workerPool.push_back(SDL_CreateThread(worker, "GenericWorker", (void*)new int(i + 1)));
	}
}

TaskQueue * TaskQueue::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new TaskQueue();
	}
	return m_instance;
}

SDL_sem * TaskQueue::canConsume()
{
	return m_canConsume;
}

std::function<void()> TaskQueue::consumeJob()
{
	SDL_LockMutex(m_processedLock);
	m_busy++;
	SDL_UnlockMutex(m_processedLock);

	SDL_LockMutex(m_queueLock);
	auto job = m_jobs.front();
	m_jobs.pop_front();
	SDL_UnlockMutex(m_queueLock);
	return job;
}

void TaskQueue::addJob(std::function<void()> f)
{
	SDL_LockMutex(m_queueLock);
	m_jobs.push_back(f);
	SDL_UnlockMutex(m_queueLock);
	SDL_SemPost(m_canConsume);
}

void TaskQueue::waitUntilIdle()
{
	while (true)
	{
		SDL_LockMutex(m_queueLock);
		if (SDL_SemValue(m_canConsume) == 0 && m_busy == 0)
		{
			SDL_UnlockMutex(m_queueLock);
			break;
		}
		SDL_UnlockMutex(m_queueLock);
	}
}

void TaskQueue::jobDone()
{
	SDL_LockMutex(m_processedLock);
	m_busy--;
	SDL_UnlockMutex(m_processedLock);
}
