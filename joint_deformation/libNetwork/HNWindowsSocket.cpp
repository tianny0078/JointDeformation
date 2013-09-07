#include "HNWindowsSocket.h"
#include <assert.h>
#include "HNLogger.h"

using namespace std;

bool HNWindowsSocket::recvPacket(HNNetworkPacket &packet) { 	
	HNLOG1("Trying to read packet")
	long size = 0;
	long bytes = ::recv(_sockfd, (char *)&size, sizeof(size), MSG_WAITALL);
	if(bytes <= 0) HNLOGGER_RBLOG4("No data read error " << WSAGetLastError(), false)
	packet.allocateFull(size);
	bytes = ::recv(_sockfd, (char *)packet.getData(), packet.size(), MSG_WAITALL);
	if(bytes != packet.size()) HNLOGGER_RBLOG4("Full packet not read " << bytes, false)
	HNLOG1("Packet of type " << packet.getPacketType() << " with timestamp " << packet.getTimestamp() <<" read size: " << packet.size())
	return true;
}

bool HNWindowsSocket::sendPacket(HNNetworkPacket &packet) {
	HNLOG1("Sending packet type " << packet.getPacketType() << " of size " << packet.size())
	packet.consolidate();
	long size = packet.size();
	long bytes = ::send(_sockfd, (char *)&size, sizeof(size), 0);
	if(bytes <= 0) HNLOGGER_RBLOG4("No data written error " << WSAGetLastError(), false)
	bytes = ::send(_sockfd, (char *)packet.getData(), packet.size(), 0);
	if(bytes <= 0) HNLOGGER_RBLOG4("No data written error " << WSAGetLastError(), false)
	if(bytes == packet.size()) 
		HNLOGGER_IFELOG1("Packet of type " << packet.getPacketType() << " with timestamp " << packet.getTimestamp() <<" written ")
		HNLOGGER_RBLOG4("Full packet not written " << bytes, false)
	return true;
}

