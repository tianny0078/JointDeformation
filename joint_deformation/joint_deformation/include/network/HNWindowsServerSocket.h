#pragma once
#include<cstdio>
#include<WinSock2.h>
#include "hnserversocket.h"
#include "HNWindowsSocket.h"
#include "HNLogger.h"

class HNWindowsServerSocket :
	public HNServerSocket, public HNWindowsSocket
{
	bool setup() HNLOGGER_ENTERED 
		if(bind(_sockfd, (sockaddr *) &_address, sizeof(sockaddr_in))< 0) HNLOGGER_ENTERED 
			perror("Socket bind error");
			return false;
		}
		if (listen(_sockfd, QUEUE_LEN) < 0) HNLOGGER_ENTERED 
			perror("Socket listen error");
			return false;
		}
		return true;
	}
public:
	HNWindowsServerSocket(HNConnectionInfo &connectionInfo):HNServerSocket(connectionInfo),HNWindowsSocket(connectionInfo){
		_address.sin_addr.s_addr = htonl(INADDR_ANY);
		setup();
	}
	
	virtual bool waitForConnection(HNSocket *&connection) HNLOGGER_ENTERED 
		connection = new HNWindowsSocket(accept(_sockfd, NULL, 0));
		return connection->isConnected();
	}
};

