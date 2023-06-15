#include "ThreadSupport.h"
//#include <algorithm>


/*
* Thread oOllection.cpp
* 
* This is the class that process context uses to track threads received via CreateThread debug event and sueperate via process id.
*/

ThreadContainer::ThreadContainer()
{
	this->View;
}

bool ThreadContainer::ProcessCreateThreadDebugEvent(LPDEBUG_EVENT Event)
{
	ThreadInsight* NewThread;
	if (Event != nullptr) 
	{
		if (Event->dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT)
		{
			if (Event->u.CreateThread.hThread != nullptr)
			{
				NewThread = new ThreadInsight(Event->u.CreateThread.hThread);
			}
			else
			{
				NewThread = new ThreadInsight(Event->dwThreadId);
			}

			if (NewThread != nullptr)
			{					

				/*
				* Test for precess of process ID for the key.  If existant, we insert our new thread into *that* vector.
				* If the PID is not there, we add a new entry+ key and insert into the new vector created for that key
				*/
				auto PID = View.find(Event->dwProcessId);
				std::vector<ThreadInsight*>* ThreadInsertLocation;
				if (PID == View.end())
				{
					ThreadInsertLocation = new std::vector<ThreadInsight*>();
					View.insert(std::pair<DWORD, std::vector<ThreadInsight*>*>(Event->dwProcessId, ThreadInsertLocation));
				}
				else
				{
					ThreadInsertLocation = (*PID).second;
				}
				ThreadInsertLocation->insert(ThreadInsertLocation->end(), NewThread);
					return true;
			}
		}
	}
	return false;
}


bool ThreadContainer::ProcessExitThreadDebugEvent(LPDEBUG_EVENT Event)
{
	/*
	* play defensive and GC the thread object if we have one
	*/

	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == EXIT_THREAD_DEBUG_EVENT)
		{
			auto PID = View.find(Event->dwProcessId);
			if (PID != View.end())
			{
				if (View[Event->dwProcessId]->size() > 0)
				{
					// remove the thread objecte sinse its ending
					auto MatchThisThreadId = [Event](ThreadInsight* i) {return (i->GetTargetThreadId() == Event->dwThreadId); };
					auto HitMe = std::find_if(View[Event->dwProcessId]->begin(), View[Event->dwProcessId]->end(), MatchThisThreadId);

					if (HitMe != View[Event->dwProcessId]->end())
					{
						delete (*HitMe);
						View[Event->dwProcessId]->erase(HitMe);
					}
				}

			}

		}
	}
	return false;
}

bool ThreadContainer::ProcessExitProcessDebugEvent(LPDEBUG_EVENT Event)
{
	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
		{
			auto PID = View.find(Event->dwProcessId);
			if (PID != View.end())
			{
				
				delete View[Event->dwProcessId];
				
				return true;
			}

		}
	}
	return false;
}

bool ThreadContainer::SetDeadThreadPreference(BOOL Purge)
{
	bool ret = this->PurgePreference;
	this->PurgePreference = Purge;
	return ret;
}

void ThreadContainer::ManualPurge()
{
	for (auto stepper = View.begin(); stepper != View.end(); stepper++)
	{
		ManualPurge((*stepper).first);
	}
}

bool is_zero(ThreadInsight* i)
{
	return i == nullptr;
}
void ThreadContainer::ManualPurge(DWORD dwProcessID)
{

}

void ThreadContainer::RemoveSingleProcess(DWORD dwProcessID)
{

}

void ThreadContainer::RemoveSingleThread(DWORD dwProcessID, DWORD dwThreadID)
{

}

ThreadInsight* ThreadContainer::GetThreadInsightPtr(DWORD dwProcessID, DWORD dwThreadID)
{
	
	auto PID = View.find(dwProcessID);
	if (PID != View.end())
	{
		auto MatchThisThreadId = [dwThreadID](ThreadInsight* i) {return (i->GetTargetThreadId() == dwThreadID); };
		auto TID = std::find_if(View[dwProcessID]->begin(), View[dwProcessID]->end(), MatchThisThreadId);

		if (TID != View[dwProcessID]->end())
		{
			return *TID;
		}
	}
	return nullptr;
}

DWORD ThreadContainer::ProcessCount()
{
	return View.size();
}

DWORD ThreadContainer::ThreadCount(DWORD ProcessId)
{
	

	return 0;
}
