#include "stdafx.h"
#include "SyncManager.h"
#include "TaskQueue.h"

SyncManager::SyncManager() 
	:	m_notifQueueLock(SDL_CreateMutex())
{

}

void SyncManager::addRecipients(std::vector<IComponent*> recipients, int componentId)
{
	m_recipientDirectory[componentId] = recipients;
}

void SyncManager::removeRecipients(int componentId)
{
	m_recipientDirectory.erase(componentId);
}

void SyncManager::registerChanges(IComponent * self, Change changes)
{
	for (auto& component : m_recipientDirectory[self->id])
	{
		if (component != self)
		{
			m_notificationQueue[SDL_ThreadID()].push_back(new ChangeNotification(changes, component, self));
		}
	}
}

void SyncManager::DistributeChanges()
{
	//to allow parallel distribution of changes, modifications to the same components
	//must either be grouped together as one, or culled by system priority/time
	//this results in a single change to each component at maximum, allowing us to parallelize the
	//distribution of changes

	//first, since we are now running sequentially, we must merge the notification queues allocated to each thread
	std::unordered_map<IComponent*, std::vector<ChangeNotification*>> notifsByObserver;
	std::vector<ChangeNotification*> notificationQueue;
	for (auto& kv : m_notificationQueue)
	{
		//grouping notifications by observer will help with next step
		for (auto& notif : kv.second)
		{
			notifsByObserver[notif->m_observer].push_back(notif);
		}
		kv.second.clear();
	}

	//then we must find duplicates and either cull or merge them appropriately
	for (auto& kv : notifsByObserver)
	{
		//only one change operation is allowed per observer
		if (kv.second.size() > 1)
		{
			//group by subject
			//if multiple notifications from same subject just merge them
			std::unordered_map<IComponent*, ChangeNotification*> notifsBySubject;
			for (auto& notif : kv.second)
			{
				auto& it = notifsBySubject.find(notif->m_subject);
				if (it != notifsBySubject.end()) // if theres duplicate notifications by subject/system
				{
					//merge them together
					it->second->mergeNotification(notif);
				}
				else
				{
					notifsBySubject[notif->m_subject] = notif;
				}
			}

			//once merging of notifications are done
			//find highest priority notifications
			//and remove notifications of lower priority
			std::vector<ChangeNotification*> notifs;
			int highestPriority = std::numeric_limits<int>::max();
			for (auto& keyVal : notifsBySubject)
			{
				int priority = keyVal.second->m_subject->priority;
				if (priority < highestPriority)
				{
					notifs.clear();
					notifs.push_back(keyVal.second);
					highestPriority = priority;
				}
				else if (priority > highestPriority)
				{
					delete keyVal.second;
				}
				else
				{
					notifs.push_back(keyVal.second);
				}
			}

			//once low priority notifications are gone
			//sort by timestamp
			std::sort(notifs.begin(), notifs.end(), [](const ChangeNotification* c1, const ChangeNotification* c2) { return c1->m_timestamp > c2->m_timestamp; });

			//remove all but the most recent
			for (auto& notif : kv.second)
			{
				if (notif != notifs.front())
				{
					delete notif;
					notif = nullptr;
				}
			}

			//cleanup
			kv.second.erase(std::remove(kv.second.begin(), kv.second.end(), nullptr), kv.second.end());
		}
		notificationQueue.push_back(kv.second.front());
	}

	//now we must run the synchronization in parallel to remove as much overhead as possible
	for (auto& notif : notificationQueue)
	{
		SINGLETON(TaskQueue)->addJob(std::bind(&ChangeNotification::send, notif));
	}

	SINGLETON(TaskQueue)->waitUntilIdle();
	for (auto& notif : notificationQueue)
	{
		delete notif;
	}
}

void SyncManager::registerThread()
{
	SDL_LockMutex(m_notifQueueLock);
	m_notificationQueue[SDL_ThreadID()]; //creates empty list
	SDL_UnlockMutex(m_notifQueueLock);
}
