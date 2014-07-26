#ifndef __ROPE_H_
#define __ROPE_H_

#include "mesh.h"
#include "GL/freeglut.h"

struct joint
{
public:
	joint()
	{
		_x = Vector3d::Zero();
		_p = Vector3d::Zero();
		_v = Vector3d::Zero();
		_xc = Vector3d::Zero();
		_f = Vector3d::Zero();
		_d = Vector3d::Zero();
	}
public:
	Vector3d _x;
	Vector3d _xc;
	Vector3d _p;
	Vector3d _v;
	Vector3d _f;
	Vector3d _d;
};

struct Segment 
{
	Segment(int a, int b){
		_begin = a;
		_end = b;
	}
	int _begin;
	int _end;
};

struct BoundingSphere{
public:
	BoundingSphere(){
		_center = Vector3d::Zero();
		_radius = 0.0;
		_level = -1;
		_segment = NULL;
	}
	BoundingSphere(const BoundingSphere &bs){
		this->_center = bs._center;
		this->_radius = bs._radius;
		this->_level = bs._level;
		this->_segment = &(*bs._segment);
	}
	void setCenterRadius(Vector3d c, double r)
	{
		_center = c;
		_radius = r;
	}
	void setLevel(int level)
	{
		_level = level;
	}

	Vector3d _center;
	double _radius;
	int _level;
	Segment * _segment;
};

struct BoundingLevel{
public:
	BoundingLevel(int a = 0)
	{
		_level = a;
	}
	BoundingLevel(const BoundingLevel &bl)
	{
		_level = bl._level;
		for (int i = 0; i < bl._spheres.size(); i++)
		{
			BoundingSphere s = bl._spheres[i];
			_spheres.push_back(s);
		}
	}
	vector<BoundingSphere> _spheres;
	int _level;
};

struct BoundingVolumeHierarchy{
public:
	BoundingVolumeHierarchy(){
		_height = -1;
	}
	int _height;
	vector<BoundingLevel> levelList;
};


struct path{
public:
	vector<Vector3d> position_list;
	vector<int> idx_before_list;
};

class Rope
{
public:
	int num_segment;
	double length;
	double radius;

	int activeJoint;
	int staticJoint;
	vector<joint> joint_list;
	vector<int> constraint_list;
	vector<path> path_list;
	int num_path;
	bool flag_path_building;
	
	//collision handling
	vector<Segment> segment_list;
	BoundingVolumeHierarchy bvh;

public:
	Rope(int num = 6, double length = 0.05, double radius = 0.01);
	void render();
	void setNum(int num);
	void setLength(double length);

	void FTL(double timestep);
	vector<Vector3d> FTL1(int begin, int end);
	vector<Vector3d> FTL2(int begin, int end);
	void setPos(int begin, int end, vector<Vector3d> pos);
	void recoverPos(int begin, int end);
	void copyPos(int begin, int end);
	void velocityCorrection(double timestep);

	//collision detection
	void buildSphereTree();
	void computeSelfCollision();

	//path
	void addPath();
	void addPos2Path(Vector3d & pos, int idx);
};

#endif