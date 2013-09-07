#pragma once
#include "network/HNNetworkCommunicator.h"
#include "network/HNFileCommunicator.h"
#include "network/HNThreadRunnable.h"
#include "mesh.h"
#include <iostream>
using namespace std;

typedef enum {
	HN_HSM_TRANSMITTER_PACKET_TYPE_HEADER,
	HN_HSM_TRANSMITTER_PACKET_TYPE_MESH,
	HN_HSM_TRANSMITTER_PACKET_TYPE_REINITIALIZATION,
	HN_HSM_TRANSMITTER_PACKET_TYPE_PAUSE,
	HN_HSM_TRANSMITTER_PACKET_TYPE_PLAY,
	HN_HSM_TRANSMITTER_PACKET_TYPE_INDENTITY,
	HN_HSM_TRANSMITTER_PACKET_TYPE_DATA_PC,
	HN_HSM_TRANSMITTER_PACKET_TYPE_READY
}HN_HSM_TRANSMITTER_PACKET_TYPE;

typedef enum{
	HN_TRANSMITTER_TYPE_NONE,
	HN_TRANSMITTER_TYPE_PC,
	HN_TRANSMITTER_TYPE_MOBILE
}HN_TRANSMITTER_TYPE;

class HSMTransmitter :public HNCommunicatorEventHandler
{
protected:
	HNCommunicator *_pComm;
public:
	HN_TRANSMITTER_TYPE _type;
	bool isClientReady;
	int index;
public:
	HSMTransmitter(HNConnectionInfo &connectionInfo, bool isCompressed) :_pComm(new HNNetworkCommunicator(connectionInfo, isCompressed))
	{
		cout << "Waiting for connection at " << connectionInfo.port << endl;
		_pComm->addEventHandler(this);
		_type = HN_TRANSMITTER_TYPE_MOBILE;
		isClientReady = false;
		index = -1;
	}

	bool startStreaming() {
		cout << "Started streaming.." << endl;
		_pComm->init();
		return _pComm->startGenerating();
	}
	
	bool stopStreaming() {
		cout << "Stopped streaming" << endl;
		return _pComm->stopGenerating();
	}

	void HSMstarted(Mesh *m)
	{
		cout << "Starting streaming mesh.." << endl;
		_pComm->sendData(m, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
	}

	void HSMupdate(Mesh *m)
	{
		//cout << "Updating deformation.." << endl;
		_pComm->sendData(m, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
	}

	void HSMupdate(Data4PC * data4PC)
	{
		_pComm->sendData(data4PC, HN_HSM_TRANSMITTER_PACKET_TYPE_DATA_PC);
	}
	void HSMinit(Data4Mobile * data4Mobile)
	{
		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_V_SIZE);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_VX);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);


		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_VY);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_VZ);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_F_SIZE);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_FX);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);


		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_FY);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_FZ);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_STRING);
		data4Mobile->setDataFlag('E');
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
		data4Mobile->setDataFlag('N');
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
	}

	void HSMupdate(Data4Mobile * data4Mobile)
	{
		//data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_UPDATE);
		//_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
		
		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_V_SIZE);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_VX);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);


		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_VY);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_VZ);
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);

		data4Mobile->setDataFlag('E');
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
		data4Mobile->setDataFlag('N');
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
		/*
		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_STRING);
		data4Mobile->setDataFlag('V');
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
		*/
	}

	void HSMupdate2(Data4Mobile * data4Mobile)
	{
		data4Mobile->setDataType(HSM_DATA_4_MOBILE_TYPE_STRING);
		data4Mobile->setDataFlag('V');
		_pComm->sendData(data4Mobile, HN_HSM_TRANSMITTER_PACKET_TYPE_MESH);
	}

	virtual void recvdData(int packetType, HNPacket * data, HNPacketMarshaller * pMarshaller)
	{
		cout << "Package received.." << endl;
		switch(packetType){
		case HN_HSM_TRANSMITTER_PACKET_TYPE_REINITIALIZATION:
			break;
		case HN_HSM_TRANSMITTER_PACKET_TYPE_INDENTITY:
			_type = HN_TRANSMITTER_TYPE_PC;
			//_type = HN_TRANSMITTER_TYPE_MOBILE;
			cout << "Client is a PC.." << endl;
			break;
		case HN_HSM_TRANSMITTER_PACKET_TYPE_READY:
			cout << "Client" << index << " is ready.." << endl;
			isClientReady = true;
			break;
		default:
			break;
		}
	}
	~HSMTransmitter()
	{
		delete(_pComm);
	}
};