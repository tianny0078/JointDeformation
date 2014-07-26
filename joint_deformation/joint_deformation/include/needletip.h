#ifndef __NEEDLETIP_H_
#define __NEEDLETIP_H_

#include "mesh.h"
#include "GL/freeglut.h"

class needleTip{
public:
	needleTip(){
		_nBegin[0] = 0.0;
		_nBegin[1] = 0.0;
		_nBegin[2] = 0.0;
		_nEnd[0] = 0.0;
		_nEnd[1] = 0.0;
		_nEnd[2] = 0.0;

		_cp = new Node();
		_gp = new Node();
	}

	void render();
	void setNeedleBeginPos(float a, float b, float c){
		_nBegin[0] = a;
		_nBegin[1] = b;
		_nBegin[2] = c;
	}
	void setNeedleEndPos(float a, float b, float c){
		_nEnd[0] = a;
		_nEnd[1] = b;
		_nEnd[2] = c;
	}
	void setIncidentCluster(Cluster * p_cluster){
		_isTouched = true;
		_pcluster = p_cluster;
		_cp->incident_cluster.push_back(_pcluster);
		_gp->incident_cluster.push_back(_pcluster);
	}


	float _nBegin[3];
	float _nEnd[3];

	//for sm
	Cluster * _pcluster;
	Node * _cp;
	Node * _gp;
	bool _isTouched;
};

#endif