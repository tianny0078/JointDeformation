#pragma once
#include <cstring>
#include "HNPacketMarshallHandler.h"
#include <cstdio>

class HNData: public HNPacketLockedMarshallHandler
{
public:
	float timestamp;
	HNData(void) : timestamp(0) {}
	HNData(const HNData &data) :timestamp(data.timestamp){}
	~HNData(void) { }
	virtual void fprint(FILE *fp) { 
		fprintf(fp, "ts: %f\n", timestamp);
	}

	virtual void reset() { 
		timestamp = 0;
	}
	
	virtual void print() { 
		fprint(stdout);
	}

	virtual void set(const HNData &data) { timestamp = data.timestamp;}
	
	virtual long marshallData(unsigned char *marshalledData) { 
		memcpy(marshalledData, &timestamp, sizeof(timestamp));
		return getMarshalledDataSize();
	}
	
	virtual long getMarshalledDataSize() { 
		return sizeof(timestamp);
	}

	virtual void unmarshallData(unsigned char *marshalledData) { 
		memcpy(&timestamp, marshalledData, sizeof(timestamp));
	}

	void lock(){}
	void unlock(){}
};
