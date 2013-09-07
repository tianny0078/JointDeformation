#pragma once
#include"HNThread.h"
#include"HNMutex.h"
#include "HNLogger.h"
#include "HNThreadRunnable.h"

class HNObjectFactory
{
protected:
	static HNObjectFactory *_factory;
	static void setDefaultFactory(HNObjectFactory *factory) HNLOGGER_ENTERED _factory = factory;}
	HNObjectFactory() HNLOGGER_ENTERED }
public:
	static HNObjectFactory* getDefaultFactory() HNLOGGER_ENTERED return _factory;}
	virtual HNThread* createThread(HNThreadRunnable *) = 0;
	virtual HNMutex* createMutex() = 0;
};
