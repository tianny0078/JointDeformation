#pragma once
#include "HNPacketMarshaller.h"
#include <vector>
#include <zlib.h>

#define LARGE_DATA_CHUNK_SIZE 50000000
class HNCompressedPacketMarshaller :
	public HNPacketMarshaller
{
	std::vector<unsigned char> _comBuffer, _uncBuffer;
public:
	HNCompressedPacketMarshaller() {
		_comBuffer.reserve(LARGE_DATA_CHUNK_SIZE);
		_uncBuffer.reserve(LARGE_DATA_CHUNK_SIZE);
		_uncBuffer.resize(LARGE_DATA_CHUNK_SIZE/2);
	}
	virtual void marshall(HNPacket *data, HNPacketLockedMarshallHandler *object) { 
		unsigned long len = 0, size;
		if(!data) HNLOGGER_RBLOG4("Marshalled receiver cant be null")
		if(object) { 
			HNLOG1("Object being marshalled");
			this->lockData();
			object->lockData();
			size = object->getMarshalledDataSize();
			_uncBuffer.resize(size);
			_comBuffer.resize(size);
			object->marshallData(&_uncBuffer[0]);
			len = LARGE_DATA_CHUNK_SIZE;
			int res = compress(&_comBuffer[0], &len, &_uncBuffer[0], size);
			_comBuffer.resize(len);
			data->allocate(len);
			memcpy(data->getData(), &_comBuffer[0], len);
			object->unlockData();
			this->unlockData();
			HNLOG1("Object marshalled");
		} else {
			data->clear();
			HNLOG3("Empty object being marshalled");
		}
	}

	virtual void unmarshall(HNPacket *data, HNPacketLockedMarshallHandler *object) { 
		unsigned long len = 0;
		if(!data) HNLOGGER_RBLOG4("Unmarshalled receiver cant be null")
		if(!object)	HNLOGGER_RBLOG3("Unmarshalled object is null")
		HNLOG1("Object being unmarshalled");
		this->lockData();
		object->lockData();
		len = LARGE_DATA_CHUNK_SIZE;
		uncompress(&_uncBuffer[0], &len, data->getData(), data->size());
		object->unmarshallData(&_uncBuffer[0]);
		object->unlockData();
		this->unlockData();
		HNLOG1("Object unmarshalled");
	}
};

