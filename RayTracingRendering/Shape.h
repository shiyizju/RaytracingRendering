

#ifndef __RayTracingRendering__SHAPE__
#define __RayTracingRendering__SHAPE__

#include "Basic.h"

class Rect3D;

class Shape
{
public:
    
	virtual double intersection(const Ray& r) = 0;
	
    virtual Rect3D boundingBox() = 0;
	
    virtual Direction getNorm(const Point3D& position) = 0;
	
    virtual Point2D getTextureCoord(const Point3D& p) = 0;
};


#endif

