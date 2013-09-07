#pragma once
#include "HNLogger.h"

class HNThreadRunnable;
class HNThread
{
protected:
	HNThreadRunnable *_runnable;
public:
	int parameter;
	HNThread() HNLOGGER_ENTERED _runnable = 0;}
	HNThread(HNThreadRunnable *runnable) HNLOGGER_ENTERED  _runnable = runnable;}
	virtual void start(int parameter) = 0;
	HNThreadRunnable *getRunnable() {return _runnable;}
	~HNThread(void) HNLOGGER_ENTERED }
};

