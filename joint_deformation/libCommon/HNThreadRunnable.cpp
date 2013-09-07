#include "HNThreadRunnable.h"
#include "HNObjectFactory.h"

void HNThreadRunnable::createAndStartThread(int index) {
	HNObjectFactory *factory = HNObjectFactory::getDefaultFactory();
	HNThread *thread = factory->createThread(this);
	_threads.push_back(thread);
	thread->start(index);
}

//void HNThreadRunnable::startAllThreads()
//{
//	for(unsigned int i=0;i<_actions.size();i++)
//		createAndStartThread(i);
//}
