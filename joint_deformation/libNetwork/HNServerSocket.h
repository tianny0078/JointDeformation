#pragma once
#include "hnsocket.h"
#define QUEUE_LEN 10

class HNServerSocket
{
public:
	HNServerSocket(HNConnectionInfo &connectionInfo){}
	HNServerSocket(void){}
	virtual bool waitForConnection(HNSocket *&connection) = 0;
	~HNServerSocket(void) {};
};

