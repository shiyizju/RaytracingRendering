
#include "Raytracer.h"
#include <vector>
#include <fstream>
#include <string>
#include <float.h>
#include <inttypes.h>
#include "basic.h"
#include "Triangle.h"
#include "primitive.h"
#include "Accelerator.h"
#include "TriangleMesh.h"
#include "Sphere.h"

const int TRACE_DEPTH   =   3;
const double BOX_MIN    = -21;
const double BOX_MAX    =  22;

Color Raytracer::_trace(const Ray& r, int depth)
{
	Color c = { 0, 0, 0 };

	if (depth>TRACE_DEPTH)
		return c;

	HitData hitData = accel.traversal(r);
	double hitT = hitData.t;
	Primitive* hitPrim = hitData.hitPrim;

    if (hitPrim == nullptr) {
		return _scene->_backgroundColor;
    }

	Point3D hit_point;
	hit_point.x = r.s.x + r.dir.x * hitT;
	hit_point.y = r.s.y + r.dir.y * hitT;
	hit_point.z = r.s.z + r.dir.z * hitT;

	Direction norm = hitPrim->s->getNorm(hit_point);

	bool out_hit = dot_product(norm, r.dir) <= 0.0;

    if (out_hit) {
		c = getColor(hitPrim, hit_point, norm);
    }
    
	if (!hitPrim->textured && hitPrim->m->reflective) {
        
		Ray refl_ray = getReflectedRay(r.dir, norm, hit_point);

		c += _trace(refl_ray, depth+1);
	}

	if (!hitPrim->textured && hitPrim->m->refraction_index > 0) {
		double idx_ratio;

		if (out_hit) {
			idx_ratio = 1 / hitPrim->m->refraction_index;
			Ray refr_ray = getRefractedRay(r.dir, norm, hit_point, idx_ratio);
			c += _trace(refr_ray, depth+1);
		}
		else {
			idx_ratio = hitPrim->m->refraction_index;
			Direction rnorm;
			rnorm.x = -norm.x;
			rnorm.y = -norm.y;
			rnorm.z = -norm.z;
			Ray refr_ray = getRefractedRay(r.dir, rnorm, hit_point, idx_ratio);
			c += _trace(refr_ray, depth+1);
		}
	}
	return c;
}

Ray Raytracer::getReflectedRay(const Direction& ray_dir, const Direction& norm, const Point3D& inte_point)
{
	double factor = 2*(ray_dir.x*norm.x + ray_dir.y*norm.y + ray_dir.z*norm.z);
	Direction refl_dir;
	refl_dir.x = ray_dir.x - factor*norm.x;
	refl_dir.y = ray_dir.y - factor*norm.y;
	refl_dir.z = ray_dir.z - factor*norm.z;

	Ray refl_ray;
	refl_ray.dir = refl_dir;
	refl_ray.s.x = inte_point.x + RAY_EPSILON*2*refl_dir.x;    //a little trick to avoid self intersection
	refl_ray.s.y = inte_point.y + RAY_EPSILON*2*refl_dir.y; 
	refl_ray.s.z = inte_point.z + RAY_EPSILON*2*refl_dir.z; 
	return refl_ray;
}

Ray Raytracer::getRefractedRay(const Direction& ray_dir, const Direction& norm, const Point3D& inte_point, double idx_ratio)
{
	double cos_i = - dot_product(ray_dir, norm);
	double cos_t;
	double squre_cos_t = 1 - idx_ratio*idx_ratio*(1-cos_i*cos_i);

    if (squre_cos_t < 0) {
		return getReflectedRay(ray_dir, norm, inte_point);
    }
        
	cos_t = sqrt(1 - idx_ratio*idx_ratio*(1-cos_i*cos_i));
	Direction refr_dir = idx_ratio*ray_dir + (idx_ratio*cos_i - cos_t)*norm;
	Ray refr_ray;
	refr_ray.dir = refr_dir;
	refr_ray.s.x = inte_point.x + RAY_EPSILON*2*refr_dir.x;    //a little trick to avoid self intersection
	refr_ray.s.y = inte_point.y + RAY_EPSILON*2*refr_dir.y; 
	refr_ray.s.z = inte_point.z + RAY_EPSILON*2*refr_dir.z; 

	return refr_ray;
}

Color Raytracer::getColor(Primitive* p, const Point3D& inter_pos, const Direction& norm)    //
{
	Color c = { 0, 0, 0};
	Color tx_c = {0, 0, 0};

	Material* m = p->m;

	if (p->textured) {
		tx_c = p->getTexture(inter_pos);
		c.r = _scene->_ambientLight.r * tx_c.r;
		c.g = _scene->_ambientLight.g * tx_c.g;
		c.b = _scene->_ambientLight.b * tx_c.b;
	}
	else {
		c.r = _scene->_ambientLight.r * m->rKa * 255;
		c.g = _scene->_ambientLight.g * m->gKa * 255;
		c.b = _scene->_ambientLight.b * m->bKa * 255;
	}

	for (int i=0; i < _scene->_pointLight.size(); i++) {
		Color tc;
		Direction l_dir(inter_pos, _scene->_pointLight[i].p);
		Ray shadow_ray;
		shadow_ray.dir = l_dir;
		shadow_ray.s = inter_pos;

		shadow_ray.s.x += RAY_EPSILON*2*shadow_ray.dir.x;    //a little trick to avoid self intersection
		shadow_ray.s.y += RAY_EPSILON*2*shadow_ray.dir.y; 
		shadow_ray.s.z += RAY_EPSILON*2*shadow_ray.dir.z; 

		double light_dis = distance(inter_pos, _scene->_pointLight[i].p);

        // The light ray may hit other object before hit the current object, aka, the shadow.
		if (!accel.hitObject(shadow_ray, light_dis)) {
            
			double cosine = norm.x*l_dir.x + norm.y*l_dir.y + norm.z*l_dir.z;
            
			if (p->textured && cosine>0) {
				tc.r = _scene->_pointLight[i].r * cosine * tx_c.r;
				tc.g = _scene->_pointLight[i].g * cosine * tx_c.g;
				tc.b = _scene->_pointLight[i].b * cosine * tx_c.b;
				c += tc;
			}
			else {
				if (cosine>0) {
					tc.r = _scene->_pointLight[i].r * m->rKd * cosine * 255;
					tc.g = _scene->_pointLight[i].g * m->gKd * cosine * 255;
					tc.b = _scene->_pointLight[i].b * m->bKd * cosine * 255;
					c += tc;
				}
				Direction h = Direction(l_dir.x+norm.x, l_dir.y+norm.y, l_dir.z+norm.z);
				double nh = norm.x*h.x + norm.y*h.y + norm.z*h.z;
				if (nh > 0)
				{
					tc.r = _scene->_pointLight[i].r * pow(nh, m->rKs) * 255;
					tc.g = _scene->_pointLight[i].g * pow(nh, m->gKs) * 255;
					tc.b = _scene->_pointLight[i].b * pow(nh, m->bKs) * 255;
					c += tc;
				}
			}
		}
	}
    
	return c;
}


