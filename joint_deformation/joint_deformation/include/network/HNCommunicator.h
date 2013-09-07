#pragma once
#include"HNSocket.h"
#include"HNNetworkObjectFactory.h"
#include <Windows.h>
#include "HNCaptureDevice.h"
#include "HNNetworkPacket.h"
#include "HNCompressedPacketMarshaller.h"
#include "HNInfo.h"
#include "HNLogger.h"

struct HNCommunicatorEventHandler{
	virtual void  recvdData(int packetType, HNPacket *data, HNPacketMarshaller *pMarshaller) = 0;
};

typedef enum {
	HN_COMMUNICATOR_PACKET_TYPE_ACK,
	HN_COMMUNICATOR_PACKET_TYPE_STANDARD,
	HN_COMMUNICATOR_PACKET_TYPE_DROPABLE
} HN_COMMUNICATOR_PACKET_TYPE;

class HNCommunicator :public HNCaptureDevice<HNCommunicatorEventHandler>
{
protected:
	HNPacketMarshaller *_pMarshaller;
	HNNetworkPacket _writePacket;
	HNThreadSafe _writeMutex;
	SYSTEMTIME _time;

	void setupTimestamp() {
		GetSystemTime(&_time);
		_writePacket.setTimestamp(((_time.wHour*60 + _time.wMinute)*60 + _time.wSecond)*1000 + _time.wMilliseconds);
	}

	void processReadPacket(HNNetworkPacket &packet) {
		for(int i=0;i<_pEventHandlers.size();i++)
			_pEventHandlers[i]->recvdData(packet.getPacketType(), &packet, _pMarshaller);
	}

	HNPacketMarshaller* createMarshaller(bool isCompressed = false){
		return isCompressed ? new  HNCompressedPacketMarshaller() : new HNPacketMarshaller();
	}

public:
	HNCommunicator():_pMarshaller(createMarshaller()) {} 
	HNCommunicator(bool isCompressed):_pMarshaller(createMarshaller(isCompressed)) {}
	HNCommunicator(HNPacketMarshaller* marshaller):_pMarshaller(marshaller) {}
	virtual bool init() {return HNCaptureDevice::init();}
	virtual void sendData(HNPacketLockedMarshallHandler *object, int packetType) {
		_writeMutex.lockData();
		_writePacket.setPacketType(packetType);
		_pMarshaller->marshall(&_writePacket,object);
		setupTimestamp();
		transmitPacket();
		_writeMutex.unlockData();
	}
	virtual void transmitPacket() = 0;
	virtual void close() = 0;
	virtual bool destroy() { close(); return true; }
	~HNCommunicator() {
		delete _pMarshaller;
	}
};

