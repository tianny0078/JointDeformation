#pragma once
#include "HNLogger.h"
#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0 * 3.14159265358979323846264338327950288)

template<class T>
struct HNPoint2 {
	T x,y;
	HNPoint2() { set(0,0);}
	HNPoint2(T x,T y) { set(x,y);}
	HNPoint2(const HNPoint2 &point) { x = point.x; y = point.y;}
	virtual void set(T x,T y) { this->x = x;this->y = y;}
	virtual void set(const HNPoint2 &point) { this->x = point.x;this->y = point.y;}
	virtual void reduce(HNPoint2 &point) { this->x -= point.x;this->y -= point.y;}
	virtual void increase(HNPoint2 &point) { this->x += point.x;this->y += point.y;}
	virtual void dot(HNPoint2 &point) { x *= point.x; y *= point.y;}
	virtual T sum() {return x + y;}
	virtual void abs() {x = ::abs(x);y = ::abs(y);}
	virtual void divide(T d) { x/=d; y/=d;}
};

template<class T>
struct HNPoint3:public HNPoint2<T> {
	T z;
	HNPoint3() { set(0,0,0);}
	HNPoint3(T x,T y, T z) { set(x,y,z);}
	HNPoint3(const HNPoint3 &point) :HNPoint2(point) { z = point.z;}
	virtual void set(T x,T y, T z) { HNPoint2::set(x,y); this->z = z;}
	virtual void set(const HNPoint3 &point) { HNPoint2::set(point); this->z = point.z;}
	virtual void reduce(HNPoint3 &point) { HNPoint2::reduce(point); this->z -= point.z;}
	virtual void increase(HNPoint3 &point) { HNPoint2::increase(point); this->z += point.z;}
	virtual void dot(HNPoint3 &point) { HNPoint2::dot(point); this->z *= point.z;}
	virtual T sum() {return HNPoint2::sum() + z;}
	virtual void abs() {HNPoint2::abs(); z = ::abs(z);}
	virtual T l1norm(HNPoint3 &point) { HNPoint3 val(point); val.reduce(*this); val.abs(); return val.sum();}
	virtual T l2norm(HNPoint3 &point) { HNPoint3 val(point); val.reduce(*this); return ::sqrt((double)(val.ssum()));}
	virtual double ssum() { return (x*x + y*y + z*z);} 
	virtual void divide(T d) { HNPoint2::divide(d); this->z /= d;}
	virtual HNPoint3 cross(HNPoint3 &point) { 
		HNPoint3 p;
		p.x = (y*point.z - z*point.y);
		p.y = (z*point.x - x*point.z);
		p.z = (x*point.y - y*point.x);
		return p;
	} 
};

template<class T>
struct HNPoint4:public HNPoint3<T> {
	T w;
	HNPoint4() { set(0,0,0,0);}
	HNPoint4(T x,T y, T z, T w) { set(x,y,z,w);}
	HNPoint4(const HNPoint4 &point) :HNPoint3(point) { w = point.w;}
	virtual void set(T x,T y, T z, T w) { HNPoint3::set(x,y,z); this->w = w;}
	virtual void set(const HNPoint4 &point) { HNPoint3::set(point); this->w = point.w;}
	virtual void reduce(HNPoint4 &point) { HNPoint3::reduce(point); this->w -= point.w;}
	virtual void increase(HNPoint4 &point) { HNPoint3::increase(point); this->w += point.w;}
	virtual void increase(HNPoint3 &point, T w) { HNPoint3::increase(point); this->w += w;}
	virtual void divide(T d) { HNPoint3::divide(d); this->w /= d;}
};

typedef HNPoint2<float> HNPoint;
typedef HNPoint3<float> HNPointXYZ;
typedef HNPoint4<float> HNPointXYZW;
typedef HNPoint3<int> HNPoint3i;

struct HNRGB {
	unsigned char r,g,b;
	HNRGB() { set(0,0,0);}
	HNRGB(unsigned char r, unsigned char g, unsigned char b) { set(r,g,b);}
	HNRGB(const HNRGB &color) { r = color.r; g = color.g; b = color.b;}
	virtual void set(unsigned char r, unsigned char g, unsigned char b) { this->r = r;this->g = g;this->b = b;}
	virtual void set(const HNRGB &color) { r = color.r; g = color.g; b = color.b;}
	virtual void set(HNPointXYZ &color) { r = (unsigned char)color.x; g = (unsigned char)color.y; b = (unsigned char)color.z;}
};

struct HNPointXYZRGB:public HNPointXYZ, public HNRGB {
	HNPointXYZRGB() { set(0,0,0,0,0,0);}
	HNPointXYZRGB(float x,float y, float z, unsigned char r, unsigned char g, unsigned char b) { set(x,y,z,r,g,b);}
	HNPointXYZRGB(const HNPointXYZRGB &point) : HNRGB(point.r,point.g,point.b), HNPointXYZ(point.x,point.y,point.z) { }
	virtual void set(float x,float y, float z, int r, int g, int b) { HNPointXYZ::set(x,y,z); HNRGB::set(r,g,b);}
	virtual void setPosition(float x, float y, float z) { HNPointXYZ::set(x,y,z);}
	virtual void setPosition(const HNPointXYZ &point) {HNPointXYZ::set(point);}
	virtual void setColor(const HNRGB &color) {HNRGB::set(color);}
	virtual void setColor(unsigned char r,unsigned char g, unsigned char b) { HNRGB::set(r,g,b);}
};

struct HNDimension {
	HNPoint topLeft, bottomRight;
	HNDimension() { set(0,0);}
	HNDimension(HNPoint size) { set(size);}
	HNDimension(float x, float y) { set(x,y);}
	HNDimension(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY) { set(topLeftX, topLeftY, bottomRightX, bottomRightY);} 
	HNDimension(HNDimension &dimension) { set(dimension); }
	void set(const float topLeftX, float topLeftY, float bottomRightX, float bottomRightY) { topLeft.x = topLeftX; topLeft.y = topLeftY; bottomRight.x = bottomRightX; bottomRight.y = bottomRightY;} 
	void set(const HNDimension &dimension) { set(dimension.topLeft.x, dimension.topLeft.y, dimension.bottomRight.x, dimension.bottomRight.y);}
	void set(HNPoint size) { set(size.x,size.y);}
	void set(float x, float y) { set(0,0,x-1,y-1);}
	float width() { return (bottomRight.x - topLeft.x) + 1;}
	float height() { return (bottomRight.y - topLeft.y) + 1;}
	float area() {return height()*width();}
	float trimX(float x) {return x - topLeft.x;}
	float trimY(float y) {return y - topLeft.y;}
	float growX(float x) {return x + topLeft.x;}
	float growY(float y) {return y + topLeft.y;}
};

struct HNTransform {
	HNPointXYZ scale, translate, rotate;
	HNTransform() {reset();}
	HNTransform(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz): scale(sx,sy,sz), translate(tx,ty,tz), rotate(rx,ry,rz) {}
	HNTransform(const HNTransform &transform):scale(transform.scale), translate(transform.translate), rotate(transform.rotate) {}
	void set(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz){ scale.set(sx,sy,sz); translate.set(tx,ty,tz); rotate.set(rx,ry,rz);}
	void set(const HNTransform &transform) {scale.set(transform.scale); translate.set(transform.translate); rotate.set(transform.rotate); }
	void reset() {scale.set(1,1,1);translate.set(0,0,0);rotate.set(0,0,0);}
	void apply(HNPointXYZ &point) {
		point.x = point.x*(sin(DEGREES_TO_RADIANS(rotate.y)) + cos(DEGREES_TO_RADIANS(rotate.z)))*scale.x + translate.x;
		point.y = point.y*(sin(DEGREES_TO_RADIANS(rotate.z)) + cos(DEGREES_TO_RADIANS(rotate.x)))*scale.y + translate.y;
		point.z = point.z*(sin(DEGREES_TO_RADIANS(rotate.x)) + cos(DEGREES_TO_RADIANS(rotate.y)))*scale.z + translate.z;
	}
};

struct HNAngleConverter {
	static HNPointXYZ convertQuatToEulerAngles(HNPointXYZW &quat) {
		HNPointXYZ angles;	
		double r1 = 2.0*quat.x*quat.x-1+2.0*quat.y*quat.y;
		double r21 = 2.0*(quat.y*quat.z-quat.x*quat.w);
		double r31 = 2.0*(quat.y*quat.w+quat.x*quat.z);
		double r32 = 2.0*(quat.z*quat.w-quat.x*quat.y);
		double r33 = 2.0*quat.x*quat.x-1+2.0*quat.w*quat.w;
		angles.x = atan2(r32, r33 );
		angles.y = -atan(r31 / sqrt(1-r31*r31) );
		angles.z = atan2(r21, r1 );
		return angles;
	}
	
	static HNPointXYZW convertEulerToQuatAngles(HNPointXYZ &angle) {
		HNPointXYZW quat; 
		quat.x = cos(angle.x/2) * cos(angle.y/2) * cos(angle.z/2) + sin(angle.x/2) * sin(angle.y/2) * sin(angle.z/2);
		quat.y = sin(angle.x/2) * cos(angle.y/2) * cos(angle.z/2) - cos(angle.x/2) * sin(angle.y/2) * sin(angle.z/2);
		quat.z = cos(angle.x/2) * sin(angle.y/2) * cos(angle.z/2) + sin(angle.x/2) * cos(angle.y/2) * sin(angle.z/2);
		quat.w = cos(angle.x/2) * cos(angle.y/2) * sin(angle.z/2) - sin(angle.x/2) * sin(angle.y/2) * cos(angle.z/2);
		return quat;
	}
	private:
		HNAngleConverter(){}
};