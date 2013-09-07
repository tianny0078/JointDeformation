#include "HNWindowsThread.h"
#include "HNLogger.h"
#include "HNThreadRunnable.h"

void _runCode(void *threader) {
	HNThread *thread = (HNThread *) threader;
	thread->getRunnable()->processThread(thread->parameter);
}

void HNWindowsThread::start(int parameter) { 
		static int x = 0;
		HNLOG3("Thread started " << ++x)
		this->parameter = parameter;
		_handle = _beginthread(_runCode, 0, (void *)this);
		int err;
		_get_errno(&err);
		if(_handle != -1) HNLOGGER_IFELOG2("Thread created successfully")
			HNLOG4("Thread creation error no " << err)

}