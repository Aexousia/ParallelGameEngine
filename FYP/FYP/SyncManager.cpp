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
	std::vector<ChangeNotification*> notificationQueue, toDelete;
	
	GroupNotificationsByObserver(notifsByObserver);

	FilterNotifications(notificationQueue, toDelete, notifsByObserver);

	//now we must run the synchronization in parallel to remove as much overhead as possible
	//batches of 50 to make things faster
	BATCH_LIST_BEGIN(notificationQueue, 50, notif)
	{
		notif->send();
	}
	BATCH_LIST_END

	for (auto& dup : toDelete)
	{
		delete dup;
		dup = nullptr;
	}
	toDelete.clear();

	SINGLETON(TaskQueue)->waitUntilIdle();
	for (auto& notif : notificationQueue)
	{
		delete notif;
	}
}

void SyncManager::GroupNotificationsByObserver(std::unordered_map<IComponent*, std::vector<ChangeNotification*>>& out)
{
	for (auto& kv : m_notificationQueue)
	{
		//grouping notifications by observer will help with next step
		for (auto& notif : kv.second)
		{
			out[notif->m_observer].push_back(notif);
		}
		kv.second.clear();
	}
}

void SyncManager::FilterNotifications(std::vector<ChangeNotification*>& wanted, std::vector<ChangeNotification*>& unwanted, std::unordered_map<IComponent*, std::vector<ChangeNotification*>>& notifsByObserver)
{
	//then we must find duplicates and either cull or merge them appropriately
	for (auto& kv : notifsByObserver)
	{
		auto& observerNotifs = kv.second;

		//only one change operation is allowed per observer
		if (observerNotifs.size() > 1)
		{
			std::vector<ChangeNotification*> notifs;

			//group by subject
			//if multiple notifications from same subject just merge them
			std::map<IComponent*, ChangeNotification*> notifsBySubject;
			for (auto& notif : observerNotifs)
			{
				auto& it = notifsBySubject.find(notif->m_subject);
				if (it != notifsBySubject.end()) // if theres duplicate notifications by subject/system
				{
					//merge them together
					it->second->mergeNotification(notif);
					unwanted.push_back(notif);
				}
				else
				{
					notifsBySubject[notif->m_subject] = notif;
					notifs.push_back(notif);
				}
			}

			//once merging of notifications are done
			//find highest priority notifications
			//and remove notifications of lower priority
			std::sort(notifs.begin(), notifs.end(), [](const ChangeNotification* c1, const ChangeNotification* c2) { return c1->m_subject->priority > c2->m_subject->priority; });
			auto& it = std::find_if(notifs.begin(), notifs.end(), [notifs](const ChangeNotification* c) { return c->m_subject->priority > notifs[0]->m_subject->priority; });

			if (it != notifs.end())
			{
				unwanted.insert(unwanted.end(), std::make_move_iterator(it), std::make_move_iterator(notifs.end()));
			}

			//once low priority notifications are gone
			//sort by timestamp
			std::sort(notifs.begin(), notifs.end(), [](const ChangeNotification* c1, const ChangeNotification* c2) { return c1->m_timestamp > c2->m_timestamp; });
			it = notifs.begin() + 1;

			if (it != notifs.end())
			{
				unwanted.insert(unwanted.end(), std::make_move_iterator(it), std::make_move_iterator(notifs.end()));
			}
			wanted.push_back(notifs.front());
		}
		else
		{
			wanted.push_back(observerNotifs.front());
		}
	}
}

void SyncManager::registerThread()
{
	SDL_LockMutex(m_notifQueueLock);
	m_notificationQueue[SDL_ThreadID()]; //creates empty list
	SDL_UnlockMutex(m_notifQueueLock);
}