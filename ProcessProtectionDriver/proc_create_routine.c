#include "stdafx.h"

//Callback routine, whenever a new process is created
VOID OnCreateProcessNotifyRoutine(PEPROCESS InProcess, HANDLE InProcessId, PPS_CREATE_NOTIFY_INFO InCreateInfo)
{
	UNREFERENCED_PARAMETER(InProcess);
	//Is process exiting?
	if (InCreateInfo == NULL)
	{
		//Remove from process tree to prevent reassigned PID collisions
		RemovePidFromTree((ULONG)HandleToLong(InProcessId));
		return;
	}
	//Is this a chrome.exe process
	if (InCreateInfo->ImageFileName->Length / 2 > wcslen(L"chrome.exe") && wcsncmp(&InCreateInfo->ImageFileName->Buffer[InCreateInfo->ImageFileName->Length / 2 - wcslen(L"chrome.exe")], L"chrome.exe", wcslen(L"chrome.exe")) == 0)
	{
		//Register it in the process tree
		RegisterProcessInTree(InCreateInfo->ParentProcessId, InProcessId);
	}
	DEBUG("PID : %d (%d)  ImageName :%wZ CmdLine : %wZ \n" ,
		HandleToLong(InProcessId) , HandleToLong(InCreateInfo->ParentProcessId) ,
		InCreateInfo->ImageFileName ,
		InCreateInfo->CommandLine
	);
}

//Add process to the tree. Check wheter it is a child or parent
VOID RegisterProcessInTree(HANDLE InParentProcessId, HANDLE InProcessId)
{
	//Is this a parent chrome.exe process
	if (InParentProcessId != NULL && !_stricmp(GetProcessNameFromPid(InParentProcessId), "chrome.exe"))
	{
		//No - add child to process tree
		AddChildPidToTree((ULONG)HandleToLong(InParentProcessId), HandleToLong(InProcessId));
		DEBUG("CHILD: %d\n" , InProcessId);
	}
	else
	{
		//Yes - add parent to process tree
		InsertPidToTree((ULONG)HandleToLong(InProcessId));
		DEBUG("PARENT: %d\n" , InProcessId);
	}
}

