#pragma once
#include"HNThread.h"
#include <process.h>
#include "HNLogger.h"

class HNWindowsThread:public HNThread
{
	uintptr_t _handle;
public:
	HNWindowsThread():HNThread() HNLOGGER_ENTERED }
	HNWindowsThread(HNThreadRunnable *runnable):HNThread(runnable) HNLOGGER_ENTERED }
	virtual void start(int parameter);
	~HNWindowsThread(void){}
};

