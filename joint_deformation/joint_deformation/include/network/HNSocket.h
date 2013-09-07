#pragma once
#include"HNConnectionInfo.h"
#include"HNNetworkPacket.h"

#include "HNLogger.h"

class HNSocket
{

protected:
	HNConnectionInfo _connectionInfo;
public:
	HNSocket() HNLOGGER_ENTERED }
	HNSocket(HNConnectionInfo &connectionInfo) HNLOGGER_ENTERED setConnectionInfo(connectionInfo);}
	void setConnectionInfo(HNConnectionInfo &connectionInfo) HNLOGGER_ENTERED  _connectionInfo.set(connectionInfo);}
	virtual bool connect() = 0;
	virtual bool recvPacket(HNNetworkPacket &packet) = 0 ;
	virtual bool sendPacket(HNNetworkPacket &packet) = 0;
	virtual bool close() = 0;
	virtual bool isConnected() = 0;
	~HNSocket(void) HNLOGGER_ENTERED }
};

