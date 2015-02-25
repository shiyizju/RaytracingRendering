//
//  Sphere.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__Sphere__
#define __RayTracingRendering__Sphere__


#include "Rect3D.h"

#include <iostream>
#include <assert.h>

class Sphere : public Shape
{
private:
    Point3D c;
    double r;
    Rect3D bound;
    
public:
    Sphere(const Point3D& center, double radius) : bound(   center.x - radius - RAY_EPSILON,
                                                            center.x + radius + RAY_EPSILON,
                                                            center.y - radius - RAY_EPSILON,
                                                            center.y + radius + RAY_EPSILON,
                                                            center.z - radius - RAY_EPSILON,
                                                            center.z + radius + RAY_EPSILON     )
    {
        c = center;
        r = radius;
    }
    
    double intersection(const Ray& ray)
    {
        double lx, ly, lz;
        lx = c.x - ray.s.x;
        ly = c.y - ray.s.y;
        lz = c.z - ray.s.z;
        double squre_l = lx*lx +ly*ly + lz*lz;
        double squre_r = r*r;
        
        double tp = lx*ray.dir.x + ly*ray.dir.y + lz*ray.dir.z;
        if (squre_l>squre_r+RAY_EPSILON && tp<-RAY_EPSILON)
            return -1;
        
        double squre_d = squre_l - tp*tp;
        if (squre_d > squre_r+RAY_EPSILON)
            return -1;
        
        double _t = sqrt(squre_r - squre_d);
        
        double t;
        if (squre_l>squre_r) {
            t = tp - _t;
        }
        else {
            t = tp + _t;
        }
        
        return t;
    }
    
    Point2D getTextureCoord(const Point3D& p)
    {
        std::cout<<"Sphere::getTextureCoord assert"<<std::endl;
        assert(false);
        exit(0);
    }
    
    Direction getNorm(const Point3D& position)
    {
        return Direction(c, position);
    }
    
    Rect3D boundingBox()
    {
        return bound;
    }
};


#endif /* defined(__RayTracingRendering__Sphere__) */
