//
//  AABB.cpp
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#include "AABB.h"

#include <iostream>
#include <assert.h>

Point2D AABB::getTextureCoord(const Point3D& p)
{
    std::cout<<"AABB::getTextureCoord assert"<<std::endl;
    assert(false);
    exit(0);
}

void AABB::_slabInter(double s, double nearSide, double farSide, double dir, double& tmin, double& tmax)
{
    if (dir>0)
    {
        tmin = (nearSide - s) / dir;
        tmax = (farSide - s) / dir;
    }
    else if (dir<0)
    {
        tmax = (nearSide - s) / dir;
        tmin = (farSide - s) / dir;
    }
    else   //r.dir.x == 0
    {
        if (nearSide - s > 0)
        {
            tmin = RAY_MAX;
            tmax = RAY_MAX;
        }
        else if (nearSide - s == 0)
        {
            tmin = 0;
            tmax = RAY_MAX;
        }
        else if (farSide - s > 0)
        {
            tmin = -RAY_MAX;
            tmax = RAY_MAX;
        }
        else if (farSide - s ==0 )
        {
            tmin = -RAY_MAX;
            tmax = 0;
        }
        else
        {
            tmin = -RAY_MAX;
            tmax = -RAY_MAX;
        }
    }
}