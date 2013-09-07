#pragma once
#include"HNThreadRunnable.h"
#include"HNEventGenerator.h"
#include"HNObjectFactory.h"
#include "HNLogger.h"

template<class T>
class HNCaptureDevice: public HNThreadRunnable, public HNEventGenerator<T>
{
protected:
	bool _capturing,_inited;
public:
	HNCaptureDevice(void):HNThreadRunnable(),HNEventGenerator(), _inited(false) {_capturing = false;}
	virtual bool init(void) { 
		if(_inited)
			return false;
		_inited = true;
		_capturing = false; 
		return true;
	}
	virtual bool startGenerating(void) HNLOGGER_ENTERED 
		if(_capturing)
			return true;
		_capturing = true; 
		startAllThreads();
		return true;
	}
	virtual bool stopGenerating(void) HNLOGGER_ENTERED _capturing = false; return true;}
	virtual bool destroy(void) = 0;
	virtual bool isCapturing() HNLOGGER_ENTERED return _capturing;}

	virtual ~HNCaptureDevice(void) {}
  
};
