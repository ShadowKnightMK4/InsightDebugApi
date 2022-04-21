#include "ThreadSupport.h"


/*
* Thread COllection.cpp
* 
* This is the class that process context uses to track threads received via CreateThread debug event and sueperate via process id.
*/

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
				//auto ExistingProcess = this->View.find(Event->dwProcessId);
				
				{
					this->View[Event->dwProcessId].insert(this->View[Event->dwProcessId].end(), NewThread);
					return TRUE;
				}
			}
		}
	}
	return false;
}


bool ThreadContainer::ProcessExitThreadDebugEvent(LPDEBUG_EVENT Event)
{

	if (Event != nullptr)
	{
		if (Event->dwDebugEventCode == EXIT_THREAD_DEBUG_EVENT)
		{
			auto homeprocess = View.find(Event->dwProcessId);
			if (homeprocess != View.end())
			{
				if (this->PurgePreference == TRUE)
				{
					RemoveSingleThread(Event->dwProcessId, Event->dwThreadId);
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
			auto homeprocess = View.find(Event->dwProcessId);
			if (homeprocess != View.end())
			{
				this->RemoveSingleProcess(Event->dwProcessId);
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
	auto exist = View.find(dwProcessID);
	std::vector<ThreadInsight*> GCCollect;
	if (exist != View.end())
	{
		for (int step = 0; step < (*exist._Ptr)._Myval.second.size(); step++)
		{
			if ((*exist).second[step]->GetTargetThreadExitCode() != STILL_ACTIVE)
			{
				GCCollect.push_back((*exist).second[step]);
				(*exist).second[step] = 0;
			}
		}
		std::remove_if((*exist._Ptr)._Myval.second.begin(), (*exist._Ptr)._Myval.second.end(), is_zero);
		// finally we delete the collected pointers
		for (int step = 0; step < GCCollect.size(); step++)
		{
			delete GCCollect[step];
		}
	}

}

void ThreadContainer::RemoveSingleProcess(DWORD dwProcessID)
{
	auto exists = View.find(dwProcessID);
	if (exists != View.end())
	{
		for (auto stepper = 0; stepper != exists._Ptr->_Myval.second.size();stepper++)
		{
			if (exists._Ptr->_Myval.second[stepper] != 0)
			{
				delete exists._Ptr->_Myval.second[stepper];
				exists._Ptr->_Myval.second[stepper] = 0;
			}
		}
		exists._Ptr->_Myval.second.clear();
		View.erase(dwProcessID);
	}

}

void ThreadContainer::RemoveSingleThread(DWORD dwProcessID, DWORD dwThreadID)
{

	auto exists = View.find(dwProcessID);
	ThreadInsight* gchold = 0;
	if (exists != View.end())
	{
		for (auto stepper = exists._Ptr->_Myval.second.begin(); stepper != exists._Ptr->_Myval.second.end(); stepper++)
		{
			if ((*stepper._Ptr)->GetTargetThreadId() == dwThreadID)
			{
				gchold = *stepper._Ptr;
				exists._Ptr->_Myval.second.erase((stepper));
				break;
			}
		}
		if (gchold != 0)
		{
			delete gchold;
		}
	}
}

ThreadInsight* ThreadContainer::GetThreadInsightPtr(DWORD dwProcessID, DWORD dwThreadID)
{
	auto exists = View.find(dwProcessID);
	
	if (exists != View.end())
	{
		for (auto step = 0; step < (*exists).second.size(); step++)
		{
			auto poss = (*exists).second[step];
			if (poss->GetTargetThreadId() == dwThreadID)
			{
				return poss;
			}
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
	auto exists = View.find(ProcessId);
	if (exists != View.end())
	{
		return (*exists).second.size();
	}
	return 0;
}
