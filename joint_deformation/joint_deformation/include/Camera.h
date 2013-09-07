#ifndef __CAMERA_H
#define __CAMERA_H

#include "GL/freeglut.h"
#include "Eigen/Dense"
#include "Eigen/Geometry"


// Initial field of view of the camera
#define DEFAULT_FOV 0.8284271

#ifndef _PI
#define	_PI 3.14159265358979323846
#endif

using namespace Eigen;

class Camera  
{
private:
	
public:
	typedef enum {PANNING, ROTATING, ZOOMING, IDLING} mov;

public:
	void Init();
	Camera() { Init(); }
	virtual ~Camera() {}
	void SetHome(const Vector3d &_pos, double _rot, const Vector3d &_rotaxis, double _fov);
	void Reset();
	void GoHome() { Reset(); }

	void Projection(double _znear, double _zfar, double _width, double _height);
	void Modelview();
	void SetGL(double _znear, double _zfar, double _width, double _height);
	void SetCoordGL(double _znear, double _zfar, double _width, double _height);
	void GetViewCoord(Vector3d &viewdir, Vector3d &updir, Vector3d &rightdir);

	void Move(double dx, double dy, double dz);
	void Rotate(const Quaternion<double> &q, double rotdist);
	void Zoom(double logzoom);

	void SaveConfig();
	void RestoreSave();

	static void Point2TrackballPos(double x, double y, Vector3d &v);
	static void Arc2Quaternion(double x1, double y1, double x2, double y2, Quaternion<double> &q);

public:
	Vector3d pos;
	AngleAxis<double> rotation;
	//double rot;
	//Vector3d rotaxis;
	double fov;
	mov status;

	Vector3d homepos;
	//double homerot;
	//Vector3d homerotaxis;
	double homefov;

	Vector3d savepos;
	AngleAxis<double> saverotation;
	AngleAxis<double> homerotation;
	//double saverot;
	//Vector3d saverotaxis;
	double savefov;

};



#endif // __CAMERA_H
