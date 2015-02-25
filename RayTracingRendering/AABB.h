//
//  AABB.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__AABB__
#define __RayTracingRendering__AABB__

#include "Shape.h"

class AABB : public Shape
{
public:
    Point3D pmin;
    Point3D pmax;
    
public:
    AABB(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
    {
        pmin.x = xmin;
        pmin.y = ymin;
        pmin.z = zmin;
        
        pmax.x = xmax;
        pmax.y = ymax;
        pmax.z = zmax;
    }
    AABB(const Point3D& p1, const Point3D& p2)
    {
        pmin.x = _min(p1.x, p2.x);
        pmin.y = _min(p1.y, p2.y);
        pmin.z = _min(p1.z, p2.z);
        
        pmax.x = _max(p1.x, p2.x);
        pmax.y = _max(p1.y, p2.y);
        pmax.z = _max(p1.z, p2.z);
    }
    AABB(const Point3D& p1, const Point3D& p2, const Point3D& p3)
    {
        pmin.x = _min(p1.x, p2.x, p3.x);
        pmin.y = _min(p1.y, p2.y, p3.y);
        pmin.z = _min(p1.z, p2.z, p3.z);
        
        pmax.x = _max(p1.x, p2.x, p3.x);
        pmax.y = _max(p1.y, p2.y, p3.y);
        pmax.z = _max(p1.z, p2.z, p3.z);
    }
    bool overlaps(const AABB& b) const
    {
        bool x = (pmax.x >= b.pmin.x) && (pmin.x <= b.pmax.x);
        bool y = (pmax.y >= b.pmin.y) && (pmin.y <= b.pmax.y);
        bool z = (pmax.z >= b.pmin.z) && (pmin.z <= b.pmax.z);
        
        return (x&&y&&z);
    }
    bool inside(const Point3D& p) const
    {
        return (p.x >= pmin.x && p.x <=pmax.x &&
                p.y >= pmin.y && p.y <=pmax.y &&
                p.z >= pmin.z && p.z <=pmax.z );
    }
    
    AABB boundingBox()
    {
        return *this;
    }
    
    Point2D getTextureCoord(const Point3D& p);
    
    Direction getNorm(const Point3D& position)
    {
        if (double_equal(position.x, pmin.x))
            return Direction(-1, 0, 0);
        else if (double_equal(position.x, pmax.x))
            return Direction(1, 0, 0);
        else if (double_equal(position.y, pmin.y))
            return Direction(0, -1, 0);
        else if (double_equal(position.y, pmax.y))
            return Direction(0, 1, 0);
        else if (double_equal(position.z, pmin.z))
            return Direction(0, 0, -1);
        else if (double_equal(position.z, pmax.z))
            return Direction(0, 0, 1);
        return Direction(0, 0, 0);
    }
    
    
    void intersect(const Ray& r, double& tmin, double& tmax)
    {
        double tminx, tminy, tminz;
        double tmaxx, tmaxy, tmaxz;
        
        _slabInter(r.s.x, pmin.x, pmax.x, r.dir.x, tminx, tmaxx);
        _slabInter(r.s.y, pmin.y, pmax.y, r.dir.y, tminy, tmaxy);
        _slabInter(r.s.z, pmin.z, pmax.z, r.dir.z, tminz, tmaxz);
        
        tmin = _max(tminx, tminy, tminz);
        tmax = _min(tmaxx, tmaxy, tmaxz);
    }
    
    double intersection(const Ray& r)
    {
        double tminx, tminy, tminz;
        double tmaxx, tmaxy, tmaxz;
        
        _slabInter(r.s.x, pmin.x, pmax.x, r.dir.x, tminx, tmaxx);
        _slabInter(r.s.y, pmin.y, pmax.y, r.dir.y, tminy, tmaxy);
        _slabInter(r.s.z, pmin.z, pmax.z, r.dir.z, tminz, tmaxz);
        
        double tmin = _max(tminx, tminy, tminz);
        double tmax = _min(tmaxx, tmaxy, tmaxz);
        
        if (tmin>tmax)
            return -1;
        if (tmin<0)
            return tmax;
        
        return tmin;
    }
    
private:
    void _slabInter(double s, double nearSide, double farSide, double dir, double& tmin, double& tmax);
};

#endif /* defined(__RayTracingRendering__AABB__) */
