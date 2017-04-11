#pragma once
#include "Singleton.h"
#include <vector>
#include "ECSInterfaces.h"
#include "../dependancies/sdl/SDL.h"
#include <unordered_map>

//this struct is just a simple data structure which encapsulates a unit 
//of change to be distributed to an observer
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
		m_changes |= toBeMerged->m_changes;

		//take newest timestamp
		if (toBeMerged->m_timestamp > m_timestamp)
		{
			m_timestamp = toBeMerged->m_timestamp;
		}
	}

	//bitmask defined by user to pick up changes
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
	SyncManager();

	void addRecipients(std::vector<IComponent*> recipients, int componentId);

	void removeRecipients(int componentId);

	void registerChanges(IComponent* self, Change changes);

	void DistributeChanges();

	//allows us to create a notification queue for each thread, allowing us to avoid synchronizing access to notificationQueue
	void registerThread();

private:
	std::unordered_map<SDL_threadID, std::vector<ChangeNotification*>> m_notificationQueue;
	std::unordered_map<int, std::vector<IComponent*>> m_recipientDirectory;
	SDL_mutex* m_notifQueueLock;
};