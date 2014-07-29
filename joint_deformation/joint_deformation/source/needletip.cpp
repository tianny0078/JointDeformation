#include "needletip.h"

void needleTip::render()
{
	//render a segment line
	glPushMatrix();
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3ub(0, 0, 255);
	glVertex3f(_nBegin[0], _nBegin[1], _nBegin[2]);
	glVertex3f(_nEnd[0], _nEnd[1], _nEnd[2]);
	glEnd();
	glLineWidth(1);
	glPopMatrix();
	
	////render cp
	//glPushMatrix();
	//glTranslatef(_cp->target_position(0), _cp->target_position(1), _cp->target_position(2));
	//glutSolidCube(0.05);
	//glPopMatrix();

	////render gp
	//glPushMatrix();
	//glTranslatef(_gp->target_position(0), _gp->target_position(1), _gp->target_position(2));
	//glutSolidCube(0.05);
	//glPopMatrix();

	//render spheres
	for (int i = 0; i <= no_inside; i++)
	{
		glPushMatrix();
		glColor3ub(0, 255, 0);
		Vector3d temp = 0.05 * _unitVector * i;
		glTranslated(_nBegin[0]+temp(0), _nBegin[1]+temp(1), _nBegin[2]+temp(2));
		glutSolidSphere(0.009,16,16);
		glPopMatrix();
	}
}