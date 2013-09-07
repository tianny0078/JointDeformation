#include "HNNetworkCommunicator.h"

#include "HNServerSocket.h"
#include "HNLogger.h"


void HNNetworkCommunicator::waitForConnection() { 
	HNLOG1("Waiting for connection " << _connectionInfo.port)
	HNServerSocket *serverSocket = HNNetworkObjectFactory::getDefaultFactory()->createServerSocket(_connectionInfo);
	serverSocket->waitForConnection(_pSocket);
	if(_pSocket) HNLOGGER_IFELOG2("Received a connection")
		HNLOG4("Connected to nobody")
	delete(serverSocket);
}

void HNNetworkCommunicator::run() { 
	HNLOG2("Netcomm started reading data");
	HNNetworkPacket packet;
	while(_pSocket && _pSocket->isConnected()) {
		if(_pSocket->recvPacket(packet))
		{
			processReadPacket(packet);
		} 
		HNLOGGER_ELOG4("Packet not read correctly")
	}
}
