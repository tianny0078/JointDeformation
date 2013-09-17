#ifndef __CUBETOOL_H_
#define __CUBETOOL_H_

#include "Kernel.h"
#include "GL/freeglut.h"

class cubeTool
{
protected:
	bool _isVisible;
	Vector3d _posBegin;
	Vector3d _posEnd;
	void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric);
public:
	cubeTool():_isVisible(false){ _posBegin.setZero(); _posEnd.setZero();}
	void render();
	void setPosBegin(float x, float y, float z);
	void setPosEnd(float x, float y, float z);
	Vector3d getPosBegin(){return _posBegin;}
	Vector3d getPosEnd(){return _posEnd;}
	void setVisible(bool a){_isVisible = a;}
};

#endif