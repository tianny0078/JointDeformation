#pragma once
#include "HNThread.h"
#include <vector>


class HNThread;

//class HNObjectFactory;
class HNThreadRunnable
{
	std::vector<void (HNThreadRunnable::*)()> _actions;
	std::vector<HNThread *> _threads;
public:
	HNThreadRunnable(void) {
		addAction(&HNThreadRunnable::run);
	}
	
	void addAction(void (HNThreadRunnable::*function)() ) {
		_actions.push_back(function);
	}

	void createAndStartThread(int index);
	
	void processThread(int index)
	{
		void (HNThreadRunnable::*function)() = _actions[index];
		((this)->*function)();
	}

	void startAllThreads()
	{
	for(unsigned int i=0;i<_actions.size();i++)
		createAndStartThread(i);
}

	virtual void run() = 0;
	~HNThreadRunnable(void) {
		for(unsigned int i=0;i<_threads.size();i++)
			delete(_threads[i]);
	}
};
