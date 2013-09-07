#pragma once
#include<cstring>
#include "HNPacket.h"
#include "HNLogger.h"


class HNNetworkPacket:public HNPacket
{
private:
	int _packetType;
	long _timestamp;
protected:
	long _dataSize;
public:
	HNNetworkPacket(void):HNPacket(),_packetType(0),_dataSize(0), _timestamp(0) {}
	HNNetworkPacket(int packetType):HNPacket(),_packetType(packetType),_dataSize(0){}
	HNNetworkPacket(const HNNetworkPacket& packet):HNPacket(packet) {
		_dataSize = size() - sizeof(_packetType)-sizeof(_timestamp);
	}
	void setPacketType(int packetType) { 
		_packetType = packetType;
		if(data.size() > 0)
			memcpy(getData() + _dataSize,&_packetType,sizeof(_packetType));
	}

	void setTimestamp(long timestamp) { 
		_timestamp = timestamp;
		if(data.size() > 0)
			memcpy(getData() +_dataSize+sizeof(_packetType),&_timestamp,sizeof(_timestamp));
	}
	
	virtual long getDataSize() { 
		return _dataSize;
	}

	virtual int getPacketType() { 
		if(data.size() > 0)
			_packetType = *((int *)((unsigned char *)getData()+_dataSize));
		return _packetType;
	}

	virtual int getTimestamp() { 
		if(data.size() > 0)
			_timestamp = *((long *)((unsigned char *)getData()+_dataSize+sizeof(_packetType)));
		return _timestamp;
	}

	virtual void clear() { 
		HNPacket::clear();
		_dataSize = 0;
	}
	
	virtual void allocate(long capacity) { 
		HNLOG1("CAP " << capacity << "SIZE " << sizeof(_packetType) << " TS " << sizeof(_timestamp));
		HNPacket::allocate(capacity+sizeof(_packetType)+sizeof(_timestamp));
		_dataSize = capacity;
		setPacketType(_packetType);
		setTimestamp(_timestamp);
	}

	virtual void allocateFull(long size) {
		HNPacket::allocate(size);
		_dataSize = size - sizeof(_packetType)-sizeof(_timestamp);
	}

	virtual void copyWithFooter(void *data, long size) { 
		HNPacket::allocate(size);
		memcpy(getData(),data,size);
		this->data.resize(size);
	}

	virtual void consolidate() {
		if(size() == 0) {
			HNPacket::allocate(sizeof(_packetType) + sizeof(_timestamp));
			_dataSize = 0;
			setTimestamp(getTimestamp());
			setPacketType(getPacketType());
		}
	}

	HNNetworkPacket& operator=(HNNetworkPacket &packet) {
		data = packet.data;
		_dataSize = size() - sizeof(_packetType)-sizeof(_timestamp);
		return *this;
	}

};

