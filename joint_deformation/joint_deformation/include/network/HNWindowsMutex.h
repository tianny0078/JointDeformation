#pragma once
#include <windows.h>
#include "HNMutex.h"
#include "HNLogger.h"

class HNWindowsMutex:public HNMutex
{
	HANDLE _handle;
public:
	HNWindowsMutex(void) HNLOGGER_ENTERED _handle = CreateMutex( NULL, FALSE, NULL );}
	void lock() HNLOGGER_ENTERED WaitForSingleObject( _handle, INFINITE );}
	void unlock() HNLOGGER_ENTERED ReleaseMutex(_handle);}
	~HNWindowsMutex(void){CloseHandle(_handle);}
};

