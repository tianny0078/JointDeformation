#pragma once
#include "network/HNThreadRunnable.h"
#include "HSMTransmitter.h"
#include "network/HNCompressedPacketMarshaller.h"
#include "network/HNPacketMarshaller.h"

class HSMReceiver: public HNCommunicatorEventHandler
{
	HNCommunicator* _pComm;
public:
	Mesh* _m;
	Data4PC * _pc;
	bool isMeshReady;
	bool isUpdateReady;
	//test
	//Data4Mobile* _mobile;

public:
	virtual bool init()
	{
		return _pComm->init();
	}
	virtual bool startGenerating()
	{
		return _pComm->startGenerating();
	}
	virtual bool stopGenerating()
	{
		return _pComm->stopGenerating();
	}
	virtual bool destroy() {
		return _pComm->destroy();
	}

	HSMReceiver(HNConnectionInfo &connectionInfo, Mesh * m, bool isCompressed):_pComm(new HNNetworkCommunicator(connectionInfo, isCompressed)), _m(m){ 
		_pComm->addEventHandler(this);
		//_mobile = new Data4Mobile(_m);
		_pc = new Data4PC(_m);
		isMeshReady = false;
	}

	virtual void pause() {
		_pComm->sendData(NULL,HN_HSM_TRANSMITTER_PACKET_TYPE_PAUSE);
	}

	virtual void play() {
		_pComm->sendData(NULL,HN_HSM_TRANSMITTER_PACKET_TYPE_INDENTITY);
	}

	void ready()
	{
		_pComm->sendData(NULL,HN_COMMUNICATOR_PACKET_TYPE_ACK);
	}

	void recvdData(int packetType, HNPacket *data, HNPacketMarshaller *pMarshaller)
	{
		switch(packetType) {
		case HN_HSM_TRANSMITTER_PACKET_TYPE_MESH:
			pMarshaller->unmarshall(data,_m);
			//pMarshaller->unmarshall(data, _mobile);
			_pComm->sendData(NULL,HN_HSM_TRANSMITTER_PACKET_TYPE_READY);
			isMeshReady = true;
			break;
		case HN_HSM_TRANSMITTER_PACKET_TYPE_DATA_PC:
			isUpdateReady = false;
			pMarshaller->unmarshall(data, _pc);
			isUpdateReady = true;
			
		/*
		for(int i=0;i<_pEventHandlers.size();i++)
			_pEventHandlers[i]->HSMupdate(this, _m);*/
		break;
	}
	}
	
	~HSMReceiver(void) { 
		delete(_pComm);
	}
};
