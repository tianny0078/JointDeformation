#pragma once
#include<windows.h>
#include"HNWindowsSocket.h"
#include"HNWindowsServerSocket.h"
#include"HNConnectionInfo.h"
#include"HNNetworkObjectFactory.h"

#include "HNLogger.h"

class HNWindowsNetworkObjectFactory:public HNNetworkObjectFactory
{
protected:
	HNWindowsNetworkObjectFactory():HNNetworkObjectFactory() HNLOGGER_ENTERED  
		WSADATA wsaData;
		if(WSAStartup( MAKEWORD(2, 0), &wsaData))
			exit(0);
	}
public:
	static void setAsDefaultFactory() HNLOGGER_ENTERED setDefaultFactory(new HNWindowsNetworkObjectFactory());}
	virtual HNServerSocket* createServerSocket(HNConnectionInfo &connectionInfo) HNLOGGER_ENTERED 
		return new HNWindowsServerSocket(connectionInfo);
	}
	virtual HNSocket* createSocket(HNConnectionInfo &connectionInfo) HNLOGGER_ENTERED 
		return new HNWindowsSocket(connectionInfo);
	}
	~HNWindowsNetworkObjectFactory() HNLOGGER_ENTERED 
		WSACleanup();
	}
};
