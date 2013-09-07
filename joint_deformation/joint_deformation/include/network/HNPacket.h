#pragma once
#include<cstring>
#include <vector>
#define MAX_DATA_BUFFER 2000000
class HNPacket
{
public:
	std::vector<unsigned char> data;

	HNPacket(void) { data.reserve(MAX_DATA_BUFFER); }
	HNPacket(const HNPacket& packet) { 
		data = packet.data;
	}
	virtual void clear() { 
		allocate(0);
	}

	virtual void copy(void *data, int size) { 
		memcpy(&(this->data[0]),data,size);
	}

	unsigned char *getData() {
		return &data[0];
	}

	long size() {
		return data.size();
	}

	virtual void allocate(long size) {data.resize(size);}
};

