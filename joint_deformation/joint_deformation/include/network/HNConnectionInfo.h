#pragma once
#define HOSTNAME_SIZE 100
#include<cstring>
#include "HNLogger.h"
#include "HNInfo.h"

typedef enum {
	HN_NETWORK_MAKE_CONNECTION,
	HN_NETWORK_WAIT_FOR_CONNECTION,
}HN_NETWORK_CONNECTION_TYPE;
class HNConnectionInfo:public HNInfo
{
private: 
	HN_NETWORK_CONNECTION_TYPE _connectionType;
public:
	//char hostname[HOSTNAME_SIZE];
	int port;
	
	
	HNConnectionInfo(void) { }
	HNConnectionInfo(HNConnectionInfo &connectionInfo) { set(connectionInfo);}
	HNConnectionInfo(char *hostname, int port) { 
		set(hostname, port);
	}

	HNConnectionInfo(int port) { 
		set("", port);
	}

	bool isWaiting() { return _connectionType == HN_NETWORK_WAIT_FOR_CONNECTION; }

	void setHostname(char *hostname) { HNInfo::setName(hostname);}
	void set(char *hostname, int port, HN_NETWORK_CONNECTION_TYPE connectionType) { HNInfo::set(hostname);this->port = port; _connectionType = connectionType;}
	void set(char *hostname, int port) { set(hostname, port, (hostname[0] == '\0') ? HN_NETWORK_WAIT_FOR_CONNECTION: HN_NETWORK_MAKE_CONNECTION);}
	void set(HNConnectionInfo &connection) HNLOGGER_ENTERED set(connection.name, connection.port, connection._connectionType);}
	~HNConnectionInfo(void) HNLOGGER_ENTERED }
};

