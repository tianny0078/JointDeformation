#pragma once

#include"HNSocket.h"
#include"HNServerSocket.h"
//#include"HNConnectionInfo.h"
#include"HNCommunicator.h"
#include"HNLogger.h"
//#include"HNNetworkCommunicator.h"
//#include"HNFileInfo.h"
//#include"HNFileCommunicator.h"

class HNNetworkCommunicator;

class HNNetworkObjectFactory
{
protected:
	static void setDefaultFactory(HNNetworkObjectFactory *factory) HNLOGGER_ENTERED __pNetFactory = factory;}
	HNNetworkObjectFactory() HNLOGGER_ENTERED }
	static HNNetworkObjectFactory* __pNetFactory;
public:
	static HNNetworkObjectFactory* getDefaultFactory() HNLOGGER_ENTERED return __pNetFactory;}
	virtual HNServerSocket* createServerSocket(HNConnectionInfo &) = 0;
	virtual HNSocket* createSocket(HNConnectionInfo &) = 0;
//	HNFileCommunicator* createCommunicator(HNFileInfo &info) { return new HNFileCommunicator(info);}
//	HNNetworkCommunicator* createCommunicator(HNConnectionInfo &info) { return new HNNetworkCommunicator(info);}
};

