#include "stdafx.h"
#include "SyncManager.h"
#include "TaskQueue.h"

SyncManager::SyncManager() 
	:	m_notifQueueLock(SDL_CreateMutex())
{
	auto& threadIds = SINGLETON(TaskQueue)->getThreadIds();
	for (auto& id : threadIds)
	{
		m_notificationQueue[id];
	}
}

void SyncManager::addRecipients(std::vector<IComponent*> recipients, int componentId)
{
	m_recipientDirectory[componentId] = recipients;
}

void SyncManager::removeRecipients(int componentId)
{
	m_recipientDirectory.erase(componentId);
}

void SyncManager::componentDeleted(IComponent * c)
{
	m_componentIdDeleted[c] = true;
}

void SyncManager::registerChanges(IComponent * self, Change changes)
{
	if (m_recipientDirectory.find(self->id) != m_recipientDirectory.end())
	{
		for (auto& component : m_recipientDirectory[self->id])
		{
			if (component != self)
			{
				m_notificationQueue[SDL_ThreadID()].push_back(ChangeNotification(changes, component, self));
			}
		}
	}
}

void SyncManager::DistributeChanges()
{
	//to allow parallel distribution of changes, modifications to the same components
	//must either be grouped together as one, or culled by system priority/time
	//this results in a single change to each component at maximum, allowing us to parallelize the
	//distribution of changes
	std::deque<ChangeNotification> notificationQueue;
	std::vector<ChangeNotification*> filteredNotifications;
	
	//first, since we are now running sequentially, we must merge the notification queues allocated to each thread
	MergeNotificationQueue(notificationQueue); //results into paramater

	//remove duplicates and stuff
	FilterNotifications(notificationQueue, filteredNotifications);

	//clear deleted map
	m_componentIdDeleted.clear();

	//now we must run the synchronization in parallel to remove as much overhead as possible
	//batches of 50 to make things faster
	BATCH_LIST_BEGIN(filteredNotifications, 1000, notif, filteredNotifications)
	{
		notif->send();
	}
	BATCH_LIST_END

	SINGLETON(TaskQueue)->waitUntilIdle();
}

void SyncManager::MergeNotificationQueue(std::deque<ChangeNotification>& out)
{
	for (auto& kv : m_notificationQueue)
	{
		out.insert(out.end(), kv.second.begin(), kv.second.end());
		kv.second.clear();
	}
}

void SyncManager::FilterNotifications(std::deque<ChangeNotification>& notifications, std::vector<ChangeNotification*>& result)
{
	//find duplicate notifications and either cull or merge them appropriately
	std::map<IComponent*, ChangeNotification*> notifsByObserver;
	
	for (auto& newNotif : notifications)
	{
		auto& observerNotification = notifsByObserver[newNotif.m_observer];

		if (m_componentIdDeleted.find(newNotif.m_observer) != m_componentIdDeleted.end())
		{
			continue;
		}

		//only one change operation is allowed per observer
		if (observerNotification)
		{
			//group by subject
			//if multiple notifications from same subject just merge them
			if (newNotif.m_subject == observerNotification->m_subject)
			{
				observerNotification->mergeNotification(newNotif);
				continue;
			}

			//once merging of notifications are done
			//find highest priority notifications
			//and remove notifications of lower priority
			if (newNotif.m_subject->priority < observerNotification->m_subject->priority)
			{
				(*observerNotification) = newNotif;
				continue;
			}
			else if (newNotif.m_subject->priority > observerNotification->m_subject->priority)
			{
				continue;
			}

			//once low priority notifications are gone
			//sort by timestamp
			if (newNotif.m_timestamp > observerNotification->m_timestamp)
			{
				(*observerNotification) = newNotif;
				continue;
			}
		}
		else
		{
			observerNotification = &newNotif;
		}

		result.push_back(observerNotification);
	}
}