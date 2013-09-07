#pragma once
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "HNLogger.h"
#include "hnsocket.h"

class HNWindowsSocket :
	public HNSocket
{
protected:
	SOCKET _sockfd;
	sockaddr_in _address;

	bool setAddr(HNConnectionInfo &connectionInfo) HNLOGGER_ENTERED 
		memset(&_address, 0, sizeof(_address));  
		_address.sin_family = AF_INET;      
		hostent *host;  
		if ((host = gethostbyname(connectionInfo.name)) == NULL) HNLOGGER_ENTERED 
			return false;
		}
		_address.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
		_address.sin_port = htons(connectionInfo.port);    
	}

	bool isValid() HNLOGGER_ENTERED 
		return (int)_sockfd >= 0;
	}

public:
	HNWindowsSocket(HNConnectionInfo &connectionInfo):HNSocket(connectionInfo) HNLOGGER_ENTERED 
		int one =1;
		_sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		//setsockopt( _sockfd, IPPROTO_TCP, TCP_EXPEDITED_1122, (const char*)&one, sizeof(one)) ; 
		setsockopt( _sockfd, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof(one)) ; 
		setAddr(connectionInfo);
	}
	
	HNWindowsSocket::HNWindowsSocket(int sockfd) : _sockfd(sockfd){}

	virtual bool connect() HNLOGGER_ENTERED 
		int err = ::connect(_sockfd, (sockaddr *) &_address, sizeof(_address));
		if(err < 0)
			HNLOGGER_IFELOG4( err << "Unable to connect error " << WSAGetLastError())
		HNLOGGER_RBLOG1("Connected successfully", true)
		return  false;
	}

	virtual bool recvPacket(HNNetworkPacket &packet);

	virtual bool sendPacket(HNNetworkPacket &packet);

	long recv(unsigned char *&buffer);
	int send(void *buffer, int bufferLen);
	virtual bool close() HNLOGGER_ENTERED 
		if(!isValid()) 
			return false;
//		shutdown(_sockfd, SD_BOTH);
		closesocket(_sockfd);
		_sockfd = -1;
		return true;
	}

	void setSock(SOCKET sockfd) HNLOGGER_ENTERED 
		_sockfd = sockfd;
	}

	virtual bool isConnected() HNLOGGER_ENTERED 
		return isValid();
	}

	~HNWindowsSocket(void) HNLOGGER_ENTERED close();}
};

