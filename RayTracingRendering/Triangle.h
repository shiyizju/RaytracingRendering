//
//  Triangle.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__Triangle__
#define __RayTracingRendering__Triangle__

#include "AABB.h"
#include "TriangleMesh.h"
#include <assert.h>

class Triangle : public Shape
{
private:
    TriangleMesh *mesh;
    int index;
    AABB bound;
    
public:
    Triangle(TriangleMesh *pMesh, int face_index) :
    bound(pMesh->vertex[pMesh->face[face_index].a],
          pMesh->vertex[pMesh->face[face_index].b],
          pMesh->vertex[pMesh->face[face_index].c])
    {
        mesh = pMesh;
        index = face_index;
    }
    double intersection(const Ray& r)
    {
        const Point3D& p0 = mesh->vertex[mesh->face[index].a];
        const Point3D& p1 = mesh->vertex[mesh->face[index].b];
        const Point3D& p2 = mesh->vertex[mesh->face[index].c];
        
        double edge1[3] = { p1.x - p0.x,  p1.y - p0.y,  p1.z - p0.z  };
        double edge2[3] = { p2.x - p0.x,  p2.y - p0.y,  p2.z - p0.z  };
        double dir[3] =   { r.dir.x, r.dir.y, r.dir.z };
        
        double pvec[3], qvec[3],
        tvec[3] = { r.s.x-p0.x, r.s.y-p0.y, r.s.z-p0.z },
        det, inv_det, u, v;
        
        _cross(pvec, dir, edge2);
        
        det = _dot(edge1, pvec);
        
        if (det < RAY_EPSILON && det> -RAY_EPSILON)
            return -1.0;
        
        inv_det = 1 / det;
        
        u = _dot(tvec, pvec)*inv_det;
        
        if ( u<-RAY_EPSILON || u>1+RAY_EPSILON)
            return -1.0;
        
        _cross(qvec, tvec, edge1);
        v = _dot(dir, qvec)*inv_det;
        
        if ( v<-RAY_EPSILON || u+v>1+RAY_EPSILON )
            return -1.0;
        
        return _dot(edge2, qvec)*inv_det;
    }
    void _cross(double dest[3], double v1[3], double v2[3])
    {
        dest[0] = v1[1]*v2[2] - v1[2]*v2[1];
        dest[1] = v1[2]*v2[0] - v1[0]*v2[2];
        dest[2] = v1[0]*v2[1] - v1[1]*v2[0];
    }
    double _dot(double v1[3], double v2[3])
    {
        return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    }
    Direction getNorm(const Point3D& p)
    {
        const Point3D& p0 = mesh->vertex[mesh->face[index].a];
        const Point3D& p1 = mesh->vertex[mesh->face[index].b];
        const Point3D& p2 = mesh->vertex[mesh->face[index].c];
        
        if (!mesh->normed)
            return cross_product(p1.x-p0.x, p1.y-p0.y, p1.z-p0.z, p2.x-p1.x, p2.y-p1.y, p2.z-p1.z);
        
        double det = deteminant(p0, p1, p2);
        double inv_det = 1/det;
        
        double a = deteminant(p, p1, p2) * inv_det;
        double b = deteminant(p0, p, p2) * inv_det;
        //double c = deteminant(p0, p1, p) * inv_det;
        double c = 1 - a - b;
        assert(det!=0.0);
        
        Direction norm = mesh->norm[mesh->face_norm[index].a]*a +
        mesh->norm[mesh->face_norm[index].b]*b +
        mesh->norm[mesh->face_norm[index].c]*c;
        
        return norm;
    }
    Point2D getTextureCoord(const Point3D& p)
    {
        Point2D coord;
        
        const Point3D& p0 = mesh->vertex[mesh->face[index].a];
        const Point3D& p1 = mesh->vertex[mesh->face[index].b];
        const Point3D& p2 = mesh->vertex[mesh->face[index].c];
        
        double det = deteminant(p0, p1, p2);
        double inv_det = 1/det;
        
        double a = deteminant(p, p1, p2) * inv_det;
        double b = deteminant(p0, p, p2) * inv_det;
        //double c = deteminant(p0, p1, p) * inv_det;
        double c =  1.0-a-b;
        assert(det!=0.0);
        
        coord.x = mesh->text_cord[mesh->face_texture[index].a].x*a +
        mesh->text_cord[mesh->face_texture[index].b].x*b +
        mesh->text_cord[mesh->face_texture[index].c].x*c;
        
        coord.y = mesh->text_cord[mesh->face_texture[index].a].y*a + 
        mesh->text_cord[mesh->face_texture[index].b].y*b +
        mesh->text_cord[mesh->face_texture[index].c].y*c;
        
        return coord;
    }
    AABB boundingBox()
    {
        return bound;
    }
private:
    GLfloat deteminant(const Point3D& p1, const Point3D& p2, const Point3D& p3)
    {
        return	p1.x * p2.y * p3.z +
        p2.x * p3.y * p1.z +
        p3.x * p1.y * p2.z -
        p1.x * p3.y * p2.z -
        p2.x * p1.y * p3.z -
        p3.x * p2.y * p1.z;
    }													 
};

#endif /* defined(__RayTracingRendering__Triangle__) */
