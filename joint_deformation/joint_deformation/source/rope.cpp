#include "rope.h"
#include "math.h"

Rope::Rope(int num, double length, double radius)
{
	num_segment = (int)pow(2.0, num);
	this->length = length;
	this->radius = radius;

	double x = 0.0;

	joint j;
	j._x(0) = 0.0;
	j._x(1) = 0.0;
	j._x(2) = 0.0;
	this->joint_list.push_back(j);

	for(int i = 1, k = 0; i < num_segment+1, k < num_segment; i++, k++)
	{
		joint j;
		j._x(0) = x + i * length;
		j._x(1) = 0.0;
		j._x(2) = 0.0;
		this->joint_list.push_back(j);

		Segment newSeg(k, i);
		segment_list.push_back(newSeg);
	}

	activeJoint = 0;
	//staticJoint = 39;
	staticJoint = -1;

	bvh._height = num;
	buildSphereTree();

	num_path = 0;
	flag_path_building = false;
}

void Rope::setNum(int num)
{
	num_segment = (int)pow(2.0, num);

	double x = 0.0;

	joint j;
	j._x(0) = 0.0;
	j._x(1) = 0.0;
	j._x(2) = 0.0;
	this->joint_list.push_back(j);

	for(int i = 1, k = 0; i < num_segment+1, k < num_segment; i++, k++)
	{
		joint j;
		j._x(0) = x + i * length;
		j._x(1) = 0.0;
		j._x(2) = 0.0;
		this->joint_list.push_back(j);

		Segment newSeg(k, i);
		segment_list.push_back(newSeg);
	}
}

void Rope::setLength(double length)
{
	this->length = length;
	double x = 0.0;
	for(int i = 0; i < joint_list.size(); i++)
	{
		joint_list[i]._x(0) = x + i * length;
		joint_list[i]._x(1) = 0.0;
		joint_list[i]._x(2) = 0.0;
	}
}

void Rope::render()
{												// Set Color To Yellow
	vector<joint>::iterator ji = joint_list.begin();
	joint * pp = &(*ji);
	
	glPushMatrix();
	glColor3ub(0, 255, 0);
	glTranslated(pp->_x.x(), pp->_x.y(), pp->_x.z());
	glutSolidSphere(0.009,16,16);
	glPopMatrix();

	int i = 0;
	ji ++;
	for ( ; ji != joint_list.end(); ++ji)
	{
		glLineWidth(3);
		glBegin(GL_LINES);
		glColor3ub(0, 0, 255);
		glVertex3f(pp->_x.x(), pp->_x.y(), pp->_x.z());
		glVertex3f(ji->_x.x(), ji->_x.y(), ji->_x.z());
		glEnd();

		pp = &(*ji);

		if (i < 8)
		{
			glPushMatrix();
			glColor3ub(0, 255, 0);
			glTranslated(pp->_x.x(), pp->_x.y(), pp->_x.z());
			glutSolidSphere(0.009,16,16);
			glPopMatrix();
		}

		i++;

	}

	if (!constraint_list.empty())
	{
	
		//active node
		pp = &joint_list[constraint_list[0]];
		glPushMatrix();
		glColor3ub(0, 255, 0);
		glTranslated(pp->_x.x(), pp->_x.y(), pp->_x.z());
		glutSolidSphere(0.009,16,16);
		glPopMatrix();

		//static node
		if (constraint_list.size() > 1)
		{
			pp = &joint_list[constraint_list[1]];
			glPushMatrix();
			glColor3ub(0, 0, 255);
			glTranslated(pp->_x.x(), pp->_x.y(), pp->_x.z());
			glutSolidSphere(0.02,16,16);
			glPopMatrix();
		}
	}

	if (! path_list.empty())
	{
		for (int i = 0; i < path_list.size(); i++)
		{
			int a = path_list[i].position_list.size()-1;
			for (int j = 0; j < a; j++)
			{
				glLineWidth(3);
				glBegin(GL_LINES);
				glColor3ub(100, 100, 100);
				glVertex3f(path_list[i].position_list[j].x(), path_list[i].position_list[j].y(), path_list[i].position_list[j].z());
				glVertex3f(path_list[i].position_list[j+1].x(), path_list[i].position_list[j+1].y(), path_list[i].position_list[j+1].z());
				glEnd();
			}
		}
	}

	glLineWidth(1);
}

vector<Vector3d> Rope::FTL1(int begin, int end)
{
	vector<Vector3d> temp;
	int a = begin;
	int s = 1;
	if(begin > end)
		s = -1;
	else if (begin == end)
		return temp;

	int b = a + s;
	while (b != (end + s))
	{
		Vector3d u = joint_list[b]._p - joint_list[a]._p;
		u = joint_list[a]._p + (u * length / u.norm());
		joint_list[b]._p = u;

		temp.push_back(u);
		a = b;
		b = a + s;
	}
	return temp;
}


vector<Vector3d> Rope::FTL2(int begin, int end)
{
	//if two joints are neighbors, directly return
	vector<Vector3d> left2right;
	if (begin == end-1)
		return left2right;
	
	left2right = FTL1(begin, end-1);
	recoverPos(begin+1, end-1);
	vector<Vector3d> right2left = FTL1(end, begin+1);
	int num = left2right.size() - 1;
	for (int i = 0; i <= num; i++)
	{
		left2right[i] = (left2right[i] + right2left[num - i]) / 2.0;
	}
	setPos(begin+1, end-1, left2right);
	return left2right;
}

void Rope::recoverPos(int begin, int end)
{
	for (int i = begin; i <= end; i++)
	{
		joint_list[i]._p = joint_list[i]._xc;
	}
}

void Rope::copyPos(int begin, int end)
{
	for (int i = begin; i <= end; i++)
	{
		joint_list[i]._xc = joint_list[i]._p;
	}
}

void Rope::setPos(int begin, int end, vector<Vector3d> pos)
{
	int j = 0;
	for (int i = begin; i <= end; i++)
	{
		joint_list[i]._p = pos[j++];
	}
}

void Rope::FTL(double timestep)
{
	//for constraint joints, set velocity to be zero
	for (int i = 0; i < constraint_list.size(); i++)
	{
		joint_list[constraint_list[i]]._v = Vector3d::Zero(); 
	}
	//p<-x
	for (int i = 0; i < joint_list.size(); i++)
	{
		joint_list[i]._p = joint_list[i]._x + joint_list[i]._v * timestep + joint_list[i]._f * timestep * timestep;
	}

	copyPos(0, joint_list.size()-1);
	//solve the constraints
	int last = constraint_list.size() - 1;
	if (constraint_list[0] > 0)
	{
		vector<Vector3d> temp = FTL1(constraint_list[0], 0);
		//setPos(0, constraint_list[0]-1, temp);
	}
	if (constraint_list[last] < joint_list.size() - 1)
	{
		vector<Vector3d> temp = FTL1(constraint_list[last], joint_list.size() - 1);
		//setPos(constraint_list[last]+1, joint_list.size()-1, temp);
	}
	int previous = constraint_list[0];
	for (int i = 1; i < constraint_list.size(); i++)
	{
		FTL2(previous, constraint_list[i]);
		previous = constraint_list[i];
	}

	//compute di = pi(new) - pi(old)
	for (int i = 0; i < joint_list.size(); i++)
	{
		joint_list[i]._d = joint_list[i]._p - joint_list[i]._xc;
	}
	//velocity correction
	//velocityCorrection(timestep);
	//x<-p
	for (int i = 0; i < joint_list.size(); i++)
	{
		joint_list[i]._x = joint_list[i]._p;
	}
}

void Rope::velocityCorrection(double timestep)
{
	//damping for velocity
	for (int i = 0; i < joint_list.size() - 1; i++)
	{
		//joint_list[i]._v = (joint_list[i]._p - joint_list[i]._x) / timestep;
		joint_list[i]._v = (joint_list[i]._p - joint_list[i]._x) - 0.9 * (joint_list[i+1]._d);
		joint_list[i]._v /= timestep; 
	}
	//friction for velocity
}

void Rope::buildSphereTree()
{
	BoundingLevel l(0);
	for (int i = 0; i < num_segment; i++)
	{
		BoundingSphere sphere;
		int a = segment_list[i]._begin;
		int b = segment_list[i]._end;
		Vector3d c = (joint_list[a]._x + joint_list[b]._x) / 2.0;
		Vector3d p = (joint_list[a]._x - joint_list[b]._x) / 2.0;
		double r = p.norm();

		sphere.setCenterRadius(c, r);
		sphere.setLevel(0);
		sphere._segment = &segment_list[i];
		l._spheres.push_back(sphere);
	}
	bvh.levelList.push_back(l);

	for (int j = 1; j <= bvh._height; j++)
	{
		BoundingLevel l(j);
		bvh.levelList.push_back(l);
		BoundingLevel * lp = &bvh.levelList.back();
		int num = bvh.levelList[j-1]._spheres.size() / 2;
		for (int k = 0; k < num; k ++)
		{
			BoundingSphere sphere;
			BoundingSphere * a = &bvh.levelList[j-1]._spheres[2 * k];
			BoundingSphere * b = &bvh.levelList[j-1]._spheres[2 * k + 1];
			//center and radius
			Vector3d p = b->_center - a->_center;
			p.normalize();
			Vector3d p1 = a->_center - a->_radius * p;
			Vector3d p2 = b->_center + b->_radius * p;
			Vector3d c = (p1 + p2) / 2.0;
			double r = (p1 - p2).norm() / 2.0;
			sphere.setCenterRadius(c, r);
			//parent and children
			lp->_spheres.push_back(sphere);
			lp->_spheres[k]._level = j;
		}
	}

	//cout << "test" << bvh.levelList[1]._spheres[0]._leftChild->_level << endl;
}

void Rope::addPath()
{
	path p;
	path_list.push_back(p);
	num_path ++;
}

void Rope::addPos2Path(Vector3d & pos, int idx)
{
	path_list[num_path-1].position_list.push_back(pos);
	path_list[num_path-1].idx_before_list.push_back(idx);
}