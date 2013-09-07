#pragma once
#include<cstring>
#include "HNMutex.h"
#include "HNObjectFactory.h"

class HNThreadSafe
{
	HNMutex *_dataMutex;
public:
	HNThreadSafe() { _dataMutex = HNObjectFactory::getDefaultFactory()->createMutex(); }
	void lockData() { _dataMutex->lock();}
	void unlockData() { _dataMutex->unlock();}
	~HNThreadSafe() { if(!_dataMutex) delete(_dataMutex);}
};

