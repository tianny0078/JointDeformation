#include "cubeTool.h"

void cubeTool::render()
{
	if (_isVisible)
	{
		glColor3f(0.0, 0.0, 1.0);
		glPushMatrix();
		glTranslatef(_posBegin[0],_posBegin[1],_posBegin[2]);
		glutSolidSphere(0.03, 16, 16);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(_posEnd[0], _posEnd[1], _posEnd[2]);
		glutSolidSphere(0.03, 16, 16);
		glPopMatrix();

		glPushMatrix();
		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		gluQuadricNormals(quadratic, GLU_SMOOTH);
		gluQuadricTexture(quadratic, GL_TRUE);
		renderCylinder(_posBegin[0],_posBegin[1],_posBegin[2],_posEnd[0], _posEnd[1], _posEnd[2],0.01,16,quadratic);
		gluDeleteQuadric(quadratic);
		glPopMatrix();
		/*
		glPushMatrix();
		glBegin(GL_LINES);
		glVertex3f(_posBegin[0],_posBegin[1],_posBegin[2]);
		glVertex3f(_posEnd[0], _posEnd[1], _posEnd[2]);
		glEnd();
		glPopMatrix();
		*/
	}
}

void cubeTool::setPosBegin(float x, float y, float z)
{
	_posBegin[0] = x;
	_posBegin[1] = y;
	_posBegin[2] = z;
}

void cubeTool::setPosEnd(float x, float y, float z)
{
	_posEnd[0] = x;
	_posEnd[1] = y;
	_posEnd[2] = z;
}

void cubeTool::renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
	float vx = x2-x1;
	float vy = y2-y1;
	float vz = z2-z1;

	//handle the degenerate case of z1 == z2 with an approximation
	if(vz == 0)
		vz = .0001;

	float v = sqrt( vx*vx + vy*vy + vz*vz );
	float ax = 57.2957795*acos( vz/v );
	if ( vz < 0.0 )
		ax = -ax;
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();

	//draw the cylinder body
	glTranslatef( x1,y1,z1 );
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	//draw the first cap
	gluQuadricOrientation(quadric,GLU_INSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glTranslatef( 0,0,v );

	//draw the second cap
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
}