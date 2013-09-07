#include "Camera.h"

// Radius of trackball
#define TRACKBALL_R 0.85f

// Initialize parameters of the camera to the usual
// (i.e. at origin, looking down negative Z axis, etc.)
void Camera::Init()
{
	pos.setZero();
	homepos.setZero();
	savepos.setZero();
		
	homerotation = AngleAxis<double>::Identity();
	saverotation = AngleAxis<double>::Identity();
	rotation = AngleAxis<double>::Identity();

	fov = homefov = savefov = DEFAULT_FOV;

	status = IDLING;
}


// Set the "home" position of the camera
void Camera::SetHome(const Vector3d &_pos,
		     double _rot,
		     const Vector3d &_rotaxis,
		     double _fov)
{
	homepos = _pos;
	homerotation.angle() = _rot;
	homerotation.axis() = _rotaxis;
	homefov = _fov;
}


// Reset the camera to the "home" position
void Camera::Reset()
{
	SaveConfig();
	pos = homepos;
	rotation = homerotation;
	fov = homefov;
}


// Save the current position of the camera for a possible future undo
void Camera::SaveConfig()
{
	savepos = pos;
	saverotation = rotation;
	savefov = fov;
}


// Set up the OpenGL projection for the current
// camera position, given screen width and height (in pixels) and
// distance to near and far planes (in world coordinates)
// { Note, this is mainly for picking }
void Camera::Projection(double _znear, double _zfar, double _width, double _height)
{
	double diag = sqrt(_width*_width + _height*_height);
	double top = _height/diag * 0.5f*tan(fov) * _znear;
	double bottom = -top;
	double right = _width/diag * 0.5f*tan(fov) * _znear;
	double left = -right;
	double neardist = _znear;
	double fardist = _zfar;
	glFrustum(left, right, bottom, top, neardist, fardist);
}

void Camera::Modelview()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(180.0f/_PI*rotation.angle(), rotation.axis()(0), rotation.axis()(1), rotation.axis()(2));
	glTranslatef(-pos(0), -pos(1), -pos(2));
}


// Set up the OpenGL projection and modelview matrices for the current
// camera position, given screen width and height (in pixels) and
// distance to near and far planes (in world coordinates)
void Camera::SetGL(double _znear, double _zfar, double _width, double _height)
{
	double diag = sqrt(_width*_width + _height*_height);
	double top = _height/diag * 0.5f*::tanf(fov) * _znear;
	double bottom = -top;
	double right = _width/diag * 0.5f*::tanf(fov) * _znear;
	double left = -right;
	double neardist = _znear;
	double fardist = _zfar;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, neardist, fardist);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(180.0f/_PI*rotation.angle(), rotation.axis()(0), rotation.axis()(1), rotation.axis()(2));
	glTranslatef(-pos(0), -pos(1), -pos(2));
}

void Camera::SetCoordGL(double _znear, double _zfar, double _width, double _height)
{
	double diag = sqrt(_width*_width + _height*_height);
	double top = _height/diag * 0.5f*::tanf(fov) * _znear;
	double bottom = -top;
	double right = _width/diag * 0.5f*::tanf(fov) * _znear;
	double left = -right;
	double neardist = _znear;
	double fardist = _zfar;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, neardist, fardist);

	glMatrixMode(GL_MODELVIEW);
	glRotatef(180.0f/_PI*rotation.angle(), rotation.axis()(0), rotation.axis()(1), rotation.axis()(2));
	glTranslatef(-pos(0), -pos(1), -pos(2));
}

// Find the vector that gives the view direction of the camera, as well as
// vectors pointing "up" and "right"
void Camera::GetViewCoord(Vector3d &viewdir, Vector3d &updir, Vector3d &rightdir)
{
	Quaternion<double> q;
	q = AngleAxis<double>(rotation);
	
	// negate w of the quaternion is equivalent to
	// changing the encoded rotation from theta to 2PI - theta
	// (i.e. rotate back theta)
	q.w() = -q.w();

	viewdir(0) = 0.0; viewdir(1) = 0.0; viewdir(2) = -1.0;

	viewdir = q*viewdir;

	updir(0) = updir(2) = 0.0;  updir(1) = 1.0;
	
	updir = q*updir;

	rightdir = viewdir.cross(updir);
}


// Translate the camera by (dx, dy, dz).  Note that these are specified
// *relative to the camera's frame*
void Camera::Move(double dx, double dy, double dz)
{
	//SaveConfig();

	Vector3d xaxis(1, 0, 0);
	Vector3d yaxis(0, 1, 0);
	Vector3d zaxis(0, 0, 1);

	Quaternion<double> currq(rotation);
	
	currq.w() = -currq.w();

	xaxis = currq*xaxis;
	yaxis = currq*yaxis;
	zaxis = currq*zaxis;

	pos(0) += xaxis(0)*dx + yaxis(0)*dy + zaxis(0)*dz;
	pos(1) += xaxis(1)*dx + yaxis(1)*dy + zaxis(1)*dz;
	pos(2) += xaxis(2)*dx + yaxis(2)*dy + zaxis(2)*dz;
}


// Rotate the camera by quaternion q, about a point lying along the camera's
// view direction, a distance "rotdist" away from the camera
void Camera::Rotate(const Quaternion<double> &q, double rotdist)
{
	Vector3d viewdir, updir, rightdir;
	GetViewCoord(viewdir, updir, rightdir);
	Vector3d rotcenter(pos(0) + rotdist*viewdir(0),
			    pos(1) + rotdist*viewdir(1),
			    pos(2) + rotdist*viewdir(2));

	rotation = q * rotation; 

	GetViewCoord(viewdir, updir, rightdir);

	pos(0) = rotcenter(0) - rotdist*viewdir(0);
	pos(1) = rotcenter(1) - rotdist*viewdir(1);
	pos(2) = rotcenter(2) - rotdist*viewdir(2);
}



// Zoom the camera (i.e. change field of view)
void Camera::Zoom(double logzoom)
{
	fov /= exp(logzoom);
}


// Convert an (x,y) normalized mouse position to a position on the trackball
void Camera::Point2TrackballPos(double x, double y, Vector3d &v)
{
	double r2 = x*x + y*y;
	double t = 0.5f * TRACKBALL_R * TRACKBALL_R;

	v[0] = x;
	v[1] = y;
	if (r2 < t)
		v[2] = sqrtf(2.0f*t - r2);
	else
		v[2] = t / sqrtf(r2);
        
	v.normalize();
}


// Takes normalized mouse positions (x1, y1) and (x2, y2) and returns a
// quaternion representing a rotation on the trackball
void Camera::Arc2Quaternion(double x1, double y1, double x2, double y2, Quaternion<double> &q)
{
	if ((x1 == x2) && (y1 == y2)) 
	{
		q.w() = 1.0;
		q.x() = 0; q.y() = 0; q.z() = 0;	
		return;
	}

	Vector3d pos1, pos2;
	Point2TrackballPos(x1, y1, pos1);
	Point2TrackballPos(x2, y2, pos2);

	Vector3d rotaxis;
	rotaxis = pos1.cross(pos2);
	rotaxis.normalize();

	double rotangle = TRACKBALL_R * sqrtf((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));

	q = AngleAxis<double>(rotangle, rotaxis);
}

void Camera::RestoreSave()
{
	pos = savepos;
	rotation = saverotation;
	fov = savefov;
}