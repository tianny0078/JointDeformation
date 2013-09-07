#pragma once

class HNMutex
{
public:
	HNMutex(void) {}
	virtual void lock() = 0;
	virtual void unlock() = 0;
	~HNMutex(void){}
};

