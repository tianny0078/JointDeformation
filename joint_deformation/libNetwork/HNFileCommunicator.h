#pragma once
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include "HNCommunicator.h"
#include "HNFileInfo.h"
#include "HNPacket.h"
#include <io.h>
#include <queue>

class HNFileCommunicator:public HNCommunicator
{

protected:
	HNFileInfo _info;
	int _fd;
	std::queue<HNNetworkPacket> _packetQueue;
	HNThreadSafe _packetQueueMutex;
	
	inline void readPacket(HNNetworkPacket *packet);
	void addQueueFunctions();
public:
	HNFileCommunicator(HNFileInfo &info, HNPacketMarshaller *marshaller):_info(info), HNCommunicator(marshaller) {
		addQueueFunctions();
	}
	HNFileCommunicator(HNFileInfo &info, bool isCompressed = false):_info(info), HNCommunicator(isCompressed) {
		addQueueFunctions();
	}
	~HNFileCommunicator(void) {
		close();
	}

	virtual bool init();

	virtual void close() {
		::close(_fd);
	}

	virtual void processWriteQueue();

	virtual void processReadQueue();

	virtual void transmitPacket();

	virtual void writePacketToFile(HNNetworkPacket &packet);

	virtual void run();
};

