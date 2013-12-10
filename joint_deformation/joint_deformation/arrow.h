#ifndef __ARROW_H_
#define __ARROW_H_

#include "kernel.h"
#include "GL/freeglut.h"

class arrow
{
public:
	bool _isVisible;
	Kernel * p_kernel;
public:
	arrow(Kernel * p_kernel);
	void render();
	void renderBox();
	void renderCone();

	void setPosBeginEnd(float x1, float y1, float z1, float x2, float y2, float z2);
	void setVisible(bool a){_isVisible = a;}
	void setScale(double scale1, double scale2);
	void setLength(double length = 0.3);
};

#endif