#include "stdafx.h"


#ifndef _DEBUG
#pragma optimize ("", off) // suppress _memset
#endif
VOID ZeroMemory(PVOID InTarget,	ULONG InByteCount)
{
	ULONG Index;
	UCHAR* Target = (UCHAR*)InTarget;

	for (Index = 0; Index < InByteCount; Index++)
	{
		*Target = 0;
		Target++;
	}
}
#ifndef _DEBUG
#pragma optimize ("", on) 
#endif

PVOID AllocMemory(BOOLEAN InZeroMemory, ULONG InSize)
{
	PVOID Result = ExAllocatePoolWithTag(NonPagedPool, InSize, 'PROT');

	if (InZeroMemory && (Result != NULL))
		ZeroMemory(Result, InSize);

	return Result;
}

VOID FreeMemory(PVOID InPointer)
{
	if (IsValidPointer(InPointer)) {
		ExFreePool(InPointer);
	}
}

VOID CopyMemory(PVOID InDest, PVOID InSource, ULONG InByteCount)
{
	ULONG Index;
	UCHAR* Dest = (UCHAR*)InDest;
	UCHAR* Src = (UCHAR*)InSource;

	for (Index = 0; Index < InByteCount; Index++)
	{
		*Dest = *Src;

		Dest++;
		Src++;
	}
}

BOOLEAN IsValidPointer(PVOID InPtr)
{
	if ((InPtr == NULL) || (InPtr == (PVOID)~0))
		return FALSE;
	return TRUE;
}

VOID LockMutex(PKGUARDED_MUTEX InMutex)
{
	KeEnterGuardedRegion();
	KeAcquireGuardedMutex(InMutex);
}

VOID UnlockMutex(PKGUARDED_MUTEX InMutex)
{
	KeReleaseGuardedMutex(InMutex);
	KeLeaveGuardedRegion();
}