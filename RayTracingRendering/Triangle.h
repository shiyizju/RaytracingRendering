//
//  Triangle.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__Triangle__
#define __RayTracingRendering__Triangle__

#include "Rect3D.h"
#include "TriangleMesh.h"
#include <assert.h>

class Triangle : public Shape {
    
private:
    TriangleMesh *mesh;
    int index;
    Rect3D bound;
    
public:
    Triangle(TriangleMesh *pMesh, int face_index) : bound(pMesh->vertex[pMesh->face[face_index].a],
                                                          pMesh->vertex[pMesh->face[face_index].b],
                                                          pMesh->vertex[pMesh->face[face_index].c])
    {
        mesh = pMesh;
        index = face_index;
    }
    
    double intersection(const Ray& r);
    
    void _cross(double dest[3], double v1[3], double v2[3]) {
        dest[0] = v1[1]*v2[2] - v1[2]*v2[1];
        dest[1] = v1[2]*v2[0] - v1[0]*v2[2];
        dest[2] = v1[0]*v2[1] - v1[1]*v2[0];
    }
    
    double _dot(double v1[3], double v2[3]) {
        return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    }
    
    Direction getNorm(const Point3D& p);
    
    Point2D getTextureCoord(const Point3D& p);
    
    Rect3D boundingBox() {
        return bound;
    }
    
private:
    GLfloat deteminant(const Point3D& p1, const Point3D& p2, const Point3D& p3) {
        return	p1.x * p2.y * p3.z +
        p2.x * p3.y * p1.z +
        p3.x * p1.y * p2.z -
        p1.x * p3.y * p2.z -
        p2.x * p1.y * p3.z -
        p3.x * p2.y * p1.z;
    }													 
};

#endif /* defined(__RayTracingRendering__Triangle__) */
