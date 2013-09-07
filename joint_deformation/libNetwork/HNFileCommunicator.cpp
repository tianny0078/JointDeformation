#include "HNFileCommunicator.h"
#include <fcntl.h>

void HNFileCommunicator::run() {
	if(_info.isWritable)
		return;
	HNNetworkPacket packet;
	long oldTimestamp = 0, totalDuration = 0;
	bool repeating = false;
	while(_fd > 0){
		try {
			readPacket(&packet);
			long duration = oldTimestamp == 0 ? 0 : (packet.getTimestamp() - oldTimestamp);
			oldTimestamp = packet.getTimestamp();
			totalDuration += duration;
			if(totalDuration < _info.startAfter || (repeating && totalDuration < _info.repeatFrom))
				continue;
			if((_info.endBefore != 0 && totalDuration > _info.endBefore) || eof(_fd)) {
				if(_info.repeat) {
					oldTimestamp = totalDuration = 0;
					repeating = true;
					lseek(_fd, 0, SEEK_SET);
					continue;
				} else {
					close();
				}
			}
			HNLOG2("read packet " << packet.getPacketType() << " time " << packet.getTimestamp())
			_packetQueueMutex.lockData();
			_packetQueue.push(packet);
			_packetQueueMutex.unlockData();
		} catch(...) {
			HNLOG4("file " << _info.name << " not properly termintated")
		}
	}
}

inline void HNFileCommunicator::readPacket( HNNetworkPacket *packet )
{
	long size;
	read(_fd, (char *)&size, sizeof(long));
	packet->allocateFull(size);
	read(_fd, (char *)packet->getData(), size);
}

void HNFileCommunicator::addQueueFunctions()
{
	if(_info.isWritable)
		addAction((void (HNThreadRunnable::*)())&HNFileCommunicator::processWriteQueue);
	else
		addAction((void (HNThreadRunnable::*)())&HNFileCommunicator::processReadQueue);
}

void HNFileCommunicator::processReadQueue()
{
	long oldTimestamp = 0;
	HNLOG4("Started processing read data")
	while(true) {
		_packetQueueMutex.lockData();
		if(!_packetQueue.empty()) {
			HNNetworkPacket packet = _packetQueue.front();
			_packetQueue.pop();
			HNLOG2("tried processing packet " << packet.getPacketType() << " time " << packet.getTimestamp())
			if(oldTimestamp < packet.getTimestamp() && oldTimestamp!=0) {
				long duration = packet.getTimestamp() - oldTimestamp;
				Sleep(duration);
			}
			HNLOG2("processing packet " << packet.getPacketType() << " time " << packet.getTimestamp())
			processReadPacket(packet);
			oldTimestamp = packet.getTimestamp();
		}
		_packetQueueMutex.unlockData();
	}
}

void HNFileCommunicator::processWriteQueue()
{
	HNLOG4("Started writing packets")
	while(true) {
		_packetQueueMutex.lockData();
		if(!_packetQueue.empty()) {
			writePacketToFile(_packetQueue.front());
			_packetQueue.pop();
		}
		_packetQueueMutex.unlockData();
	}
}

void HNFileCommunicator::transmitPacket()
{
	_packetQueueMutex.lockData();
	_packetQueue.push(_writePacket);
	_packetQueueMutex.unlockData();
}

void HNFileCommunicator::writePacketToFile( HNNetworkPacket &packet )
{
	if(_info.isWritable && _fd > 0) {
		long size = packet.size();
		write(_fd, (char *)&size, sizeof(long));
		write(_fd, (char *)packet.getData(), sizeof(char)*size);
		_commit(_fd);
	}
}

bool HNFileCommunicator::init()
{
	if(HNCommunicator::init()) {
		_fd = open(_info.getFilePath(), (_info.isWritable ? (O_WRONLY | O_TRUNC | O_CREAT) : O_RDONLY) | O_BINARY);
		if(_fd > 0)
			HNLOGGER_RBLOG3("file " << _info.name << " openned successfully", true)
			HNLOGGER_ELOG4("unable to open file " << _info.name << " Error " << GetLastError())
	}
	return false;
}
