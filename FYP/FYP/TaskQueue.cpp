#include "stdafx.h"
#include "TaskQueue.h"

void TaskQueue::decrementActiveWorkers()
{
	SDL_LockMutex(m_activeWorkersLock);
	m_numActiveWorkers--;
	
	if (m_numActiveWorkers < 1)
	{
		m_numActiveWorkers = 1;
	}
	else
	{
		SDL_SemTryWait(m_workerSlotsFree);
	}
	SDL_UnlockMutex(m_activeWorkersLock);
}

void TaskQueue::incrementActiveWorkers()
{
	SDL_LockMutex(m_activeWorkersLock);
	m_numActiveWorkers++;
	if (m_numActiveWorkers > m_maxActiveWorkers)
	{
		m_numActiveWorkers = m_maxActiveWorkers;
	}
	else
	{
		SDL_SemPost(m_workerSlotsFree);
	}
	SDL_UnlockMutex(m_activeWorkersLock);
}

int TaskQueue::getNumActiveWorkers()
{
	SDL_LockMutex(m_activeWorkersLock);
	int activeWorkers = m_numActiveWorkers;
	SDL_UnlockMutex(m_activeWorkersLock);
	return activeWorkers;
}

bool TaskQueue::isActive(int workerId)
{
	SDL_LockMutex(m_activeWorkersLock);
	int activeWorkers = m_numActiveWorkers;
	SDL_UnlockMutex(m_activeWorkersLock);
	return workerId <= activeWorkers;
}

TaskQueue::TaskQueue() 
	:	m_canConsume(SDL_CreateSemaphore(0))
	,	m_queueLock(SDL_CreateMutex())
	,	m_processedLock(SDL_CreateMutex())
	,	m_activeWorkersLock(SDL_CreateMutex())
	,	m_maxActiveWorkers(std::thread::hardware_concurrency())
	,	m_numActiveWorkers(0)
	,	threadingActive(true)
	,	m_workerSlotsFree(SDL_CreateSemaphore(0))
	,	m_idle(SDL_CreateSemaphore(0))
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
		m_workerPool.push_back(SDL_CreateThread(worker, "GenericWorker", (void*)new int(i)));
		m_numActiveWorkers++;
		SDL_SemPost(m_workerSlotsFree);
	}
}

SDL_sem * TaskQueue::canConsume()
{
	return m_canConsume;
}

SDL_sem * TaskQueue::workerSlotFree()
{
	return m_workerSlotsFree;
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

	if (threadingActive)
	{
		SDL_SemPost(m_canConsume);
	}
	else
	{
		auto& func = consumeJob();
		func();
		jobDone();
	}
}

void TaskQueue::waitUntilIdle()
{
	SDL_SemWait(m_idle);
}

void TaskQueue::jobDone()
{
	SDL_LockMutex(m_processedLock);
	m_busy--;
	SDL_LockMutex(m_queueLock);
	if (m_jobs.empty() && m_busy == 0)
	{
		SDL_SemPost(m_idle);
		std::cout << "F" << std::endl;
	}
	SDL_UnlockMutex(m_queueLock);
	SDL_UnlockMutex(m_processedLock);

	if (SDL_SemValue(m_workerSlotsFree) < m_numActiveWorkers)
	{
		SDL_SemPost(m_workerSlotsFree);
	}


}
