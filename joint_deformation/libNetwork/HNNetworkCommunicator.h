#pragma once
#include"HNSocket.h"
#include"HNNetworkObjectFactory.h"
//#include"HNCaptureDevice.h"
#include "HNCompressedPacketMarshaller.h"
#include"HNCommunicator.h"
#include"HNThreadSafe.h"
#include "HNLogger.h"


//struct HNNetworkCommunicatorEventHandler {
//	virtual void recvdData(int packetType, HNPacket *data) = 0;
//};
/*public HNCaptureDevice<HNNetworkCommunicatorEventHandler>,*/ 

class HNNetworkCommunicator: public HNCommunicator  
{
protected:
	HNSocket *_pSocket;
	HNConnectionInfo _connectionInfo;
	virtual bool connect() {
		if(!_pSocket)
			_pSocket = HNNetworkObjectFactory::getDefaultFactory()->createSocket(_connectionInfo);
		if(_pSocket->connect())
			HNLOGGER_IFELOG2("Connected successfully")
			HNLOGGER_RBLOG4("Connection error to " << _connectionInfo.name << " at " << _connectionInfo.port, false)
		return true;
	}
	virtual void waitForConnection();
public:
	HNNetworkCommunicator(HNConnectionInfo &connectionInfo):_connectionInfo(connectionInfo) {_pSocket = NULL; }
	HNNetworkCommunicator(HNConnectionInfo &connectionInfo, HNPacketMarshaller *marshaller):HNCommunicator(marshaller), _connectionInfo(connectionInfo) {_pSocket = NULL; }
	HNNetworkCommunicator(HNConnectionInfo &connectionInfo, bool isCompressed):HNCommunicator(isCompressed), _connectionInfo(connectionInfo) {_pSocket = NULL; }
	HNNetworkCommunicator(HNSocket *socket):_pSocket(socket) {}
	HNNetworkCommunicator(HNSocket *socket, HNPacketMarshaller *marshaller):_pSocket(socket), HNCommunicator(marshaller){}
	HNNetworkCommunicator(HNSocket *socket, bool isCompressed):_pSocket(socket), HNCommunicator(isCompressed){}
	
	virtual bool init() {
		HNCommunicator::init();
		if(_connectionInfo.isWaiting()) {
			waitForConnection();
		} else {
			return connect();
		}
		return true;
	}

	
	virtual void transmitPacket() { 
		_pSocket->sendPacket(_writePacket);
	}	

	void run();
	virtual void close() { _pSocket->close(); }
	~HNNetworkCommunicator(void) HNLOGGER_ENTERED delete(_pSocket); delete(_pMarshaller);}
};

