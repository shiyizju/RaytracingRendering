

#ifndef __RayTracingRendering__SHAPE__
#define __RayTracingRendering__SHAPE__

#include "Basic.h"

class AABB;

class Shape
{
public:
	virtual double intersection(const Ray& r) = 0;
	virtual AABB boundingBox() = 0;
	virtual Direction getNorm(const Point3D& position) = 0;
	virtual Point2D getTextureCoord(const Point3D& p) = 0;
};


#endif

