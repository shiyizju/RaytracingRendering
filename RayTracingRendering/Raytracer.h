
#ifndef __RT_RAYTRACER_H
#define __RT_RAYTRACER_H

#include <vector>
#include <fstream>
#include <string>
#include <float.h>
#include <inttypes.h>

#include "basic.h"
#include "primitive.h"
#include "Accelerator.h"
#include "Scene.h"

extern const int TRACE_DEPTH;
extern const double BOX_MIN;
extern const double BOX_MAX;

class Raytracer
{
private:
    
	Accelerator accel;
    
    Scene *_scene;
    
public:
    
	Raytracer(Scene *scene) : accel(&scene->_primitive) {
        _scene = scene;
        accel.subdivide(Rect3D(BOX_MIN, BOX_MAX, BOX_MIN, BOX_MAX, BOX_MIN, BOX_MAX));
	}
    
    void getEyePosition(int &eyex, int &eyey, int &eyez) {
        eyex = _scene->_eyex;
        eyey = _scene->_eyey;
        eyez = _scene->_eyez;
    }
    
	Color trace(Ray r) {
		return _trace(r, 1);
	}
    /*
    Color colorOfPoint(double x, double y, double z) {
        
//        Ray r(_scene->_eyex, _scene->_eyey, _scene->_eyez, );
    }*/

private:
    
	Color _trace(const Ray& r, int depth);

	Ray getReflectedRay(const Direction& ray_dir, const Direction& norm, const Point3D& inte_point);

	Ray getRefractedRay(const Direction& ray_dir, const Direction& norm, const Point3D& inte_point, double idx_ratio);

	Color getColor(Primitive* p, const Point3D& inter_pos, const Direction& norm);    //采用phong 光照模型

	void Parse(char* filename);
};

#endif
