#pragma once
#include "HNNetworkCommunicator.h"
#include "HNLogger.h"

template<class T>
class HNNetworkReceiver : public HNNetworkCommunicatorEventHandler, public T
{
	HNNetworkCommunicator _networkComm;
public:
	HNNetworkReceiver(HNConnectionInfo &connectionInfo):_networkComm(connectionInfo){_networkComm.connect();}
	virtual bool init() HNLOGGER_ENTERED _networkComm.init();}
	virtual bool startGenerating() HNLOGGER_ENTERED _networkComm.startGenerating();}
	virtual bool stopGenerating() HNLOGGER_ENTERED _networkComm.stopGenerating();}
	virtual bool destroy() HNLOGGER_ENTERED _networkComm.destroy();}
	~HNNetworkReceiver(void){}
};