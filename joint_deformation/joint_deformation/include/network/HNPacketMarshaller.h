#pragma once
#include "HNPacketMarshallHandler.h"
#include "HNPacket.h"
#include "HNLogger.h"
#define MAX_DATA_SIZE 2000000
class HNPacketMarshaller:public HNThreadSafe
{
public:
	virtual void marshall(HNPacket *data, HNPacketLockedMarshallHandler *object) { 
		if(!data) HNLOGGER_RBLOG4("Marshalled receiver cant be null")
		if(object) { 
			HNLOG1("Object being marshalled");
			object->lockData();
			data->allocate(object->getMarshalledDataSize());
			object->marshallData(data->getData());
			object->unlockData();
			HNLOG1("Object marshalled");
		} else {
			data->clear();
			HNLOG3("Empty object being marshalled");
		}
	}

	virtual void unmarshall(HNPacket *data, HNPacketLockedMarshallHandler *object) { 
		if(!data) HNLOGGER_RBLOG4("Unmarshalled receiver cant be null")
		if(!object)	HNLOGGER_RBLOG3("Unmarshalled object is null")
		HNLOG1("Object being unmarshalled");
		object->lockData();
		object->unmarshallData(data->getData());
		object->unlockData();
		HNLOG1("Object unmarshalled");
	}
};

