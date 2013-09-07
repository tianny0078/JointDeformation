#pragma once
#include "HNWindowsThread.h"
#include "HNWindowsMutex.h"
#include "HNObjectFactory.h"
#include "HNLogger.h"


class HNWindowsObjectFactory: public HNObjectFactory
{
	
public:
	static void setAsDefaultFactory() HNLOGGER_ENTERED _factory = new HNWindowsObjectFactory();}
	virtual HNThread* createThread(HNThreadRunnable *runnable) HNLOGGER_ENTERED 
		return new HNWindowsThread(runnable);
	}
	virtual HNMutex* createMutex() HNLOGGER_ENTERED 
		return new HNWindowsMutex();
	}
};
