
#ifndef __RayTracingRendering_BASIC
#define __RayTracingRendering_BASIC

#include <OpenGL/gltypes.h>
#include <math.h>

extern const double RAY_EPSILON;
extern const double RAY_MAX;

struct Point2D {
    double x;
    double y;
};

struct Point3D {
	double x;
	double y;
	double z;
};

struct Size2D {
    float width;
    float height;
};

struct Size3D {
    float sizex;
    float sizey;
    float sizez;
};

inline Point3D operator-(const Point3D& p0, const Point3D& p1) {
	Point3D p;
	p.x = p0.x - p1.x;
	p.y = p0.y - p1.y;
	p.z = p0.z - p1.z;
	return p;
}

inline Point3D operator*(const double factor, const Point3D& p) {
	Point3D fp;
	fp.x = factor*p.x;
	fp.y = factor*p.y;
	fp.z = factor*p.z;
	return fp;
}

inline Point3D operator*(const Point3D& p, const double factor) {
	Point3D fp;
	fp.x = factor*p.x;
	fp.y = factor*p.y;
	fp.z = factor*p.z;
	return fp;
}

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

struct Light {
	Point3D p;
	double r;
	double g;
	double b;
};

struct Material
{
	double rKa, rKd, rKs;
	double gKa, gKd, gKs;
	double bKa, bKd, bKs;
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	bool reflective;
	double refraction_index;
};

struct Direction
{
	double x, y, z;

	Direction() {

	}

	Direction(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;

		double s = sqrt(x*x + y*y + z*z);

		if (s!=0)
		{
			this->x /= s;
			this->y /= s;
			this->z /= s;
		}
	}

	Direction(const Point3D& p0, const Point3D& p1)   //from p0 to p1
	{
		Point3D p = p1 - p0;

		x = p.x;
		y = p.y;
		z = p.z;

		double s = sqrt(x*x + y*y + z*z);

		if (s!=0)
		{
			this->x /= s;
			this->y /= s;
			this->z /= s;
		}
	}
};

inline Direction operator*(const double factor, const Direction& d) {
	Direction dir;
	dir.x = d.x*factor;
	dir.y = d.y*factor;
	dir.z = d.z*factor;
	return dir;
}

inline Direction operator*(const Direction& d, const double factor) {
	Direction dir;
	dir.x = d.x*factor;
	dir.y = d.y*factor;
	dir.z = d.z*factor;
	return dir;
}

inline Direction operator+(const Direction& d1, const Direction& d2) {
	Direction dir;
	dir.x = d1.x + d2.x;
	dir.y = d1.y + d2.y;
	dir.z = d1.z + d2.z;
	return dir;
}

struct Ray
{
	Point3D s;
	Direction dir;

	Ray() : dir() {
        
	}

	Ray(double x0, double y0, double z0, double x1, double y1, double z1) :
	dir(x1-x0, y1-y0, z1-z0)
	{
		s.x = x0;
		s.y = y0;
		s.z = z0;
	}

	Ray(const Point3D& p0, const Point3D& p1) : dir(p0, p1)
	{
		s = p0;
	}
};

inline Direction cross_product(double ax, double ay, double az, double bx, double by, double bz)
{
	double x, y, z;
	x = ay*bz - az*by;
	y = az*bx - ax*bz;
	z = ax*by - ay*bx;
	return Direction(x, y, z);
}

inline double dot_product(double ax, double ay, double az, double bx, double by, double bz)
{
	return ax*bx + ay*by + az*bz;
}

inline double dot_product(const Direction& d1, const Direction& d2)
{
	return d1.x*d2.x + d1.y*d2.y + d1.z*d2.z;
}

inline double dot_product(const Point3D& p1, const Point3D& p2)
{
	return p1.x*p2.x + p1.y*p2.y + p1.z*p2.z;
}

inline double dot_product(const Point3D& p, const Direction& d)
{
	return p.x*d.x + p.y*d.y + p.z*d.z;
}

inline double dot_product(const Direction& d, const Point3D& p)
{
	return p.x*d.x + p.y*d.y + p.z*d.z;
}

inline bool double_equal(double d1, double d2)
{
	if (fabs(d1-d2)<=RAY_EPSILON)
		return true;
	return false;
}

inline bool double_larger(double d1, double d2)
{
	if (d1-d2 > RAY_EPSILON)
		return true;
	return false;
}

inline void swap(double&d1, double &d2)
{
	double tmp = d1;
	d1 = d2;
	d2 = tmp;
}

inline bool double_smaller(double d1, double d2)
{
	if (d2-d1 > RAY_EPSILON)
		return true;
	return false;
}

inline double _min(double d1, double d2)
{
	if (d1>d2)
		return d2;
	return d1;
}
inline double _max(double d1, double d2)
{
	if (d1>d2)
		return d1;
	return d2;
}
inline double _min(double d1, double d2, double d3)
{
	double d = _min(d1, d2);
	return _min(d, d3);
}
inline double _max(double d1, double d2, double d3)
{
	double d = _max(d1, d2);
	return _max(d, d3);
}

inline double double_divide(double d1, double d2)
{
	if (d2!=0.0)
		return d1/d2;

	if (d1>0.0)
		return RAY_MAX;
	if (d1<0.0)
		return -RAY_MAX;
	return 0.0;
}

inline Color operator+(const Color& c1, const Color& c2)
{
	Color c;
    
    short s;
    s = c1.r + c2.r;
    if (s > 255) { c.r = 255; } else { c.r = s; }
    s = c1.g + c2.g;
    if (s > 255) { c.g = 255; } else { c.g = s; }
    s = c1.b + c2.b;
    if (s > 255) { c.b = 255; } else { c.b = s; }

	return c;
}

inline void operator+=(Color& c1, const Color& c2)
{
    short s;
    
    s = c1.r + c2.r;
    if (s > 255) { c1.r = 255; } else { c1.r = s; }
    s = c1.g + c2.g;
    if (s > 255) { c1.g = 255; } else { c1.g = s; }
    s = c1.b + c2.b;
    if (s > 255) { c1.b = 255; } else { c1.b = s; }
}

inline double distance(const Point3D& p0, const Point3D& p1)
{
	double dx = p1.x - p0.x;
	double dy = p1.y - p0.y;
	double dz = p1.z - p0.z;

	return sqrt(dx*dx+dy*dy+dz*dz);
}

inline Point3D cross(const Point3D& p1, const Point3D& p2)
{
	Point3D p;
	p.x = p1.y*p2.z - p1.z*p2.y;
	p.y = p1.z*p2.x - p1.x*p2.z;
	p.z = p1.x*p2.y - p1.y*p2.x;
	return p;
}

#endif
