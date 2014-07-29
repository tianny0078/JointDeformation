#ifndef __NEEDLETIP_H_
#define __NEEDLETIP_H_

#include "mesh.h"
#include "GL/freeglut.h"

class needleTip{
public:
	needleTip(){
		_nBegin = Vector3d::Zero();
		_nEnd = Vector3d::Zero();
		_nTrueBegin = Vector3d::Zero();
		_nLastTrueBegin = Vector3d::Zero();
		_unitVector = Vector3d::Zero();

		_cp = new Node();
		_gp = new Node();
		_cp->mass = 10.0;
		_gp->mass = 10.0;

		_isPuntured = false;
		_isTouched = false;
		_pcluster = NULL;
		no_related = 0;
		no_inside = 0;
	}

	void render();
	void setNeedleBeginPos(float a, float b, float c){
		_nBegin(0) = a;
		_nBegin(1) = b;
		_nBegin(2) = c;

		//compute _nEnd
		Vector3d temp = 0.05 * _unitVector * 30;
		_nEnd = _nBegin + temp;
	}

	void setIncidentCluster(Cluster * p_cluster){
		_pcluster = p_cluster;
		_cp->incident_cluster.push_back(_pcluster);
		_gp->incident_cluster.push_back(_pcluster);
	}


	Vector3d _nBegin;
	Vector3d _nEnd;
	Vector3d _unitVector;
	Vector3d _nTrueBegin;
	Vector3d _nLastTrueBegin;
	int no_related;
	int no_inside;

	//for sm
	Cluster * _pcluster;
	Node * _cp;
	Node * _gp;
	bool _isPuntured;
	bool _isTouched;
};

#endif