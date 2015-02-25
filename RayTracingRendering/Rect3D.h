//
//  Rect3D.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__Rect3D__
#define __RayTracingRendering__Rect3D__

#include "Shape.h"

class Rect3D : public Shape {
    
private:
    Point3D pmin;
    Point3D pmax;
    
    friend class Accelerator;
    
public:
    
    Rect3D(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
        
        pmin.x = xmin;
        pmin.y = ymin;
        pmin.z = zmin;
        
        pmax.x = xmax;
        pmax.y = ymax;
        pmax.z = zmax;        
    }
    
    Rect3D(const Point3D& p1, const Point3D& p2) {
        
        pmin.x = _min(p1.x, p2.x);
        pmin.y = _min(p1.y, p2.y);
        pmin.z = _min(p1.z, p2.z);
        
        pmax.x = _max(p1.x, p2.x);
        pmax.y = _max(p1.y, p2.y);
        pmax.z = _max(p1.z, p2.z);
    }
    
    Rect3D(const Point3D& p1, const Point3D& p2, const Point3D& p3) {
        
        pmin.x = _min(p1.x, p2.x, p3.x);
        pmin.y = _min(p1.y, p2.y, p3.y);
        pmin.z = _min(p1.z, p2.z, p3.z);
        
        pmax.x = _max(p1.x, p2.x, p3.x);
        pmax.y = _max(p1.y, p2.y, p3.y);
        pmax.z = _max(p1.z, p2.z, p3.z);
    }
    
    bool isOverlap(const Rect3D& aRect) const {
        
        if (double_larger(pmin.x, aRect.pmax.x) || double_smaller(pmax.x, aRect.pmin.x)) {
            return false;
        }
        if (double_larger(pmin.y, aRect.pmax.y) || double_smaller(pmax.y, aRect.pmin.y)) {
            return false;
        }
        if (double_larger(pmin.z, aRect.pmax.z) || double_smaller(pmax.z, aRect.pmin.z)) {
            return false;
        }
        return true;
    }
    
    bool isPointInside(const Point3D& aPoint) const {
        
        if ( double_larger(pmin.x, aPoint.x) || double_smaller(pmax.x, aPoint.x)) {
            return false;
        }
        if ( double_larger(pmin.y, aPoint.y) || double_smaller(pmax.y, aPoint.y)) {
            return false;
        }
        if ( double_larger(pmin.z, aPoint.z) || double_smaller(pmax.z, aPoint.z)) {
            return false;
        }
        
        return true;
    }
    
    Rect3D boundingBox() {
        return *this;
    }
    
    Point2D getTextureCoord(const Point3D& p);
    
    Direction getNorm(const Point3D& position) {
        
        if (double_equal(position.x, pmin.x)) {
            return Direction(-1, 0, 0);
        }
        else if (double_equal(position.x, pmax.x)) {
            return Direction(1, 0, 0);
        }
        else if (double_equal(position.y, pmin.y)) {
            return Direction(0, -1, 0);
        }
        else if (double_equal(position.y, pmax.y)) {
            return Direction(0, 1, 0);
        }
        else if (double_equal(position.z, pmin.z)) {
            return Direction(0, 0, -1);
        }
        else if (double_equal(position.z, pmax.z)) {
            return Direction(0, 0, 1);
        }
        
        return Direction(0, 0, 0);
    }
    
    void intersect(const Ray& r, double& tmin, double& tmax) {
        
        double tminx, tminy, tminz;
        double tmaxx, tmaxy, tmaxz;
        
        _slabIntersection(r.s.x, pmin.x, pmax.x, r.dir.x, tminx, tmaxx);
        _slabIntersection(r.s.y, pmin.y, pmax.y, r.dir.y, tminy, tmaxy);
        _slabIntersection(r.s.z, pmin.z, pmax.z, r.dir.z, tminz, tmaxz);
        
        tmin = _max(tminx, tminy, tminz);
        tmax = _min(tmaxx, tmaxy, tmaxz);
    }
    
    double intersection(const Ray& r) {
        
        double tminx, tminy, tminz;
        double tmaxx, tmaxy, tmaxz;
        
        _slabIntersection(r.s.x, pmin.x, pmax.x, r.dir.x, tminx, tmaxx);
        _slabIntersection(r.s.y, pmin.y, pmax.y, r.dir.y, tminy, tmaxy);
        _slabIntersection(r.s.z, pmin.z, pmax.z, r.dir.z, tminz, tmaxz);
        
        double tmin = _max(tminx, tminy, tminz);
        double tmax = _min(tmaxx, tmaxy, tmaxz);
        
        if (tmin > tmax) {
            return -1;
        }
        
        if (tmin < 0) {
            return tmax;
        }
        
        return tmin;
    }
    
private:
    
    // Intersection with one paticular slab, aka, xy, xz, or yz.
    void _slabIntersection(double s, double nearSide, double farSide, double dir, double& tmin, double& tmax);
    
};

#endif /* defined(__RayTracingRendering__Rect3D__) */
