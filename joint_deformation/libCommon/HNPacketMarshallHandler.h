#pragma once
#include "HNThreadSafe.h"

class HNPacketMarshallHandler
{
public:
	virtual long marshallData(unsigned char *marshalledData) = 0;
	virtual long getMarshalledDataSize() = 0;
	virtual void unmarshallData(unsigned char *marshalledData) = 0;
};

class HNPacketLockedMarshallHandler : public HNThreadSafe, public HNPacketMarshallHandler {};