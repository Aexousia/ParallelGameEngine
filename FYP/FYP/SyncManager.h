#pragma once
#include "Singleton.h"
#include <vector>
#include "ECSInterfaces.h"
#include "../dependancies/sdl/SDL.h"
#include <unordered_map>

struct ChangeNotification
{
	ChangeNotification(Change c, IComponent* observer, IComponent* subject)
		:	m_changes(c)
		,	m_observer(observer)
		,	m_subject(subject)
		,	m_timestamp(SDL_GetTicks())
	{

	}

	void send()
	{
		m_observer->ChangeOccured(m_changes, m_subject);
	}

	void mergeNotification(ChangeNotification* toBeMerged)
	{
		//merge change bitmask

		//take newest timestamp
	}

	Change m_changes;
	IComponent* m_observer;
	IComponent *m_subject;
	uint32_t m_timestamp;
};


// This class helps to both register and distribute changes sent in by the different threads
// since each system has copies of each component, these component's data needs
// synchronizing when changes occur in 1 system but not another
class SyncManager : public Singleton<SyncManager>
{
public:
	SyncManager() : m_notifQueueLock(SDL_CreateMutex())
	{

	}

	void addRecipients(std::vector<IComponent*> recipients, int componentId)
	{
		m_recipientDirectory[componentId] = recipients;
	}

	void removeRecipients(int componentId)
	{
		m_recipientDirectory.erase(componentId);
	}

	void registerChanges(IComponent* self, Change changes)
	{
		for (auto& component : m_recipientDirectory[self->id])
		{
			if (component != self)
			{
				m_notificationQueue[SDL_ThreadID()].push_back(new ChangeNotification(changes, component, self));
			}
		}
	}

	void DistributeChanges()
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
			if (kv.second.size() > 1)
			{
				//group by subject
				//if multiple notifications from same subject just merge them
				std::unordered_map<IComponent*, ChangeNotification*> notifsBySubject;
				for (auto& notif : kv.second)
				{
					auto& it = notifsBySubject.find(notif->m_subject);
					if (it != notifsBySubject.end())
					{
						it->second->mergeNotification(notif);
						delete it->second;
					}
					else
					{
						notifsBySubject[notif->m_subject] = notif;
					}
				}

				//once merging of notifications are done
				//find highest priority notification


				//remove notifications of lower priority


				//once low priority notifications are gone
				//sort by timestamp


				//remove all but the most recent

			}
			notificationQueue.push_back(kv.second.front());
		}
		//now we must run the synchronization in parallel to remove as much overhead as possible

	}

	//allows us to create a notification queue for each thread, allowing us to avoid synchronizing access to notificationQueue
	void registerThread()
	{
		SDL_LockMutex(m_notifQueueLock);
		m_notificationQueue[SDL_ThreadID()]; //creates empty list
		SDL_UnlockMutex(m_notifQueueLock);
	}

private:
	std::unordered_map<SDL_threadID, std::vector<ChangeNotification*>> m_notificationQueue;
	std::unordered_map<int, std::vector<IComponent*>> m_recipientDirectory;
	SDL_mutex* m_notifQueueLock;
};
