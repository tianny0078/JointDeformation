#include "needletip.h"

void needleTip::render()
{
	//render a segment line
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3ub(0, 0, 255);
	glVertex3f(_nBegin[0], _nBegin[1], _nBegin[2]);
	glVertex3f(_nEnd[0], _nEnd[1], _nEnd[2]);
	glEnd();
	glLineWidth(1);
}