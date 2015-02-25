
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
	std::vector<Primitive*> pPrim;
	Light* pLights[5];          // Max light count is 5, including ambient light.

	int nPrim;
	int nLight;
    
	Accelerator accel;
    
    Scene *_scene;
    
public:
	Raytracer(char* scenefile) : accel(&pPrim)
	{
		nPrim = 0;
		nLight = 0;
        
        _scene = new Scene;

		Light *ambient = new Light;
		ambient->r = 0.1;
		ambient->g = 0.1;
		ambient->b = 0.1;
		pLights[nLight++] = ambient;

		Parse(scenefile);
	}

	Color trace(Ray r)
	{
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
