#include "arrow.h"

arrow::arrow(Kernel * p):_isVisible(true)
{
	p_kernel = p;
}

void arrow::render()
{
	renderBox();
	renderCone();
}

void arrow::renderBox()
{
	for (int i = 0; i < p_kernel->f_box_list.size(); ++i)
	{
		Vector3d n0, n1, n2, n01, n02, n;

		n0 = p_kernel->_rotation * p_kernel->v_box_list[p_kernel->f_box_list[i](0)] + p_kernel->_posBegin;
		n1 = p_kernel->_rotation * p_kernel->v_box_list[p_kernel->f_box_list[i](1)] + p_kernel->_posBegin;
		n2 = p_kernel->_rotation * p_kernel->v_box_list[p_kernel->f_box_list[i](2)] + p_kernel->_posBegin;

		n01 = n0 - n1;
		n02 = n0 - n2;

		n = n01.cross(n02);
		n.normalize();

		//display a line mesh
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glBegin(GL_TRIANGLES);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3d(n0[0], n0[1], n0[2]);
		glVertex3d(n1[0], n1[1], n1[2]);
		glVertex3d(n2[0], n2[1], n2[2]);
		glEnd();
	}
}

void arrow::renderCone()
{
	for (int i = 0; i < p_kernel->f_cone_list.size(); ++i)
	{
		Vector3d n0, n1, n2, n01, n02, n;

		n0 = p_kernel->_rotation * p_kernel->v_cone_list[p_kernel->f_cone_list[i](0)] + p_kernel->_posEnd;
		n1 = p_kernel->_rotation * p_kernel->v_cone_list[p_kernel->f_cone_list[i](1)] + p_kernel->_posEnd;
		n2 = p_kernel->_rotation * p_kernel->v_cone_list[p_kernel->f_cone_list[i](2)] + p_kernel->_posEnd;

		n01 = n0 - n1;
		n02 = n0 - n2;

		n = n01.cross(n02);
		n.normalize();

		//display a line mesh
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glBegin(GL_TRIANGLES);
		glNormal3d(n[0], n[1], n[2]);
		glVertex3d(n0[0], n0[1], n0[2]);
		glVertex3d(n1[0], n1[1], n1[2]);
		glVertex3d(n2[0], n2[1], n2[2]);
		glEnd();
	}
}

void arrow::setScale(double scale1, double scale2)
{
	for (int i = 0; i < p_kernel->v_box_list.size(); ++i)
	{
		p_kernel->v_box_list[i](0) = p_kernel->v_box_list[i](0) * scale1;
		p_kernel->v_box_list[i](2) = p_kernel->v_box_list[i](2) * scale1;
	}

	for (int i = 0; i < p_kernel->v_cone_list.size(); ++i)
	{
		p_kernel->v_cone_list[i](0) = p_kernel->v_cone_list[i](0) * scale2;
		p_kernel->v_cone_list[i](2) = p_kernel->v_cone_list[i](2) * scale2;
	}
}

void arrow::setLength(double length)
{
	p_kernel->v_cone_list[4](1) = length;
}


void arrow::setPosBeginEnd(float x1, float y1, float z1, float x2, float y2, float z2)
{
	p_kernel->_posBegin(0) =  x1;
	p_kernel->_posBegin(1) =  y1;
	p_kernel->_posBegin(2) =  z1;

	p_kernel->_posEnd(0) = x1 + 4*(x2 - x1) ;
	p_kernel->_posEnd(1) = y1 + 4*(y2 - y1);
	p_kernel->_posEnd(2) = z1 + 4*(z2 - z1);

	Vector3d temp = p_kernel->_posEnd - p_kernel->_posBegin;
	for (int i = 4; i < p_kernel->v_box_list.size(); ++i)
	{
		p_kernel->v_box_list[i](1) = temp.norm();
	}

	Vector3d v1, v2, axis;
	v1(0) = x2 - x1;
	v1(1) = y2 - y1;
	v1(2) = z2 - z1;
	v1.normalize();

	v2(0) = 0;
	v2(1) = 1;
	v2(2) = 0;

	//cout << v1 << endl;

	axis = v1.cross(v2);
	axis.normalize();
	double angle = (-1) * acos(v1(1));

	p_kernel->_rotation.setZero();
	p_kernel->_rotation(0, 0) = cos(angle) + axis(0) * axis(0) * (1 - cos(angle));
	p_kernel->_rotation(0, 1) = axis(0) * axis(1) * (1 - cos(angle)) - axis(2) * sin(angle);
	p_kernel->_rotation(0, 2) = axis(0) * axis(2) * (1 - cos(angle)) + axis(1) * sin(angle);
	p_kernel->_rotation(1, 0) = axis(1) * axis(0) * (1 - cos(angle)) + axis(2) * sin(angle);
	p_kernel->_rotation(1, 1) = cos(angle) + axis(1) * axis(1) * (1 - cos(angle));
	p_kernel->_rotation(1, 2) = axis(1) * axis(2) * (1 - cos(angle)) - axis(0) * sin(angle);
	p_kernel->_rotation(2, 0) = axis(2) * axis(0) * (1 - cos(angle)) - axis(1) * sin(angle);
	p_kernel->_rotation(2, 1) = axis(2) * axis(1) * (1 - cos(angle)) + axis(0) * sin(angle);
	p_kernel->_rotation(2, 2) = cos(angle) + axis(2) * axis(2) * (1 - cos(angle));
	//cout << (_rotation * v2) << endl;
}