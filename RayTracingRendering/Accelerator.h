

#ifndef __RayTracingRendering__ACCELERATOR__
#define __RayTracingRendering__ACCELERATOR__

#include "basic.h"
#include "primitive.h"

#include <float.h>
#include <vector>

extern const int MAX_TREEDEPTH;
extern const int PRIM_LIMIT;

struct BSPAccelNode {
    
    // spliting axis.
	enum { X, Y, Z } splitAxis;
    
    // node type.
	enum { INNER, LEAF } type;
    
	Rect3D box;
    
	BSPAccelNode *left;
	BSPAccelNode *right;
	
    Primitive **refPrim;
	int nPrim;

	BSPAccelNode() : box(0, 0, 0, 0, 0, 0) {
		left = nullptr;
		right = nullptr;
		refPrim = nullptr;
		nPrim = 0;
	}
};

struct HitData
{
	Primitive *hitPrim;
	double t;
};

class Accelerator {
    
public:
	BSPAccelNode* root;
	std::vector<Primitive*>* pPrims;

public:
	Accelerator(std::vector<Primitive*>* prim_vector) {
		pPrims = prim_vector;
		root = NULL;
	}

	bool hitObject(Ray r, double light_dist) {
		double tmin, tmax;
		root->box.intersect(r, tmin, tmax);

        if (tmin > tmax) {
			return false;
        }

		return _hitObject(r, root, tmin, tmax, light_dist);
	}
	
	HitData traversal(Ray r) {
		Rect3D box = root->box;

		double tmin, tmax;
		box.intersect(r, tmin, tmax);

		if (tmin > tmax)
		{
			std::cout<<"box no intersection"<<std::endl;
			HitData d;
			d.hitPrim = NULL;
			d.t = -1;
			return d;
		}

		return _traversal(r, root, tmin, tmax);
	}

	void subdivide(Rect3D box)
	{
		root = new BSPAccelNode;
		root->box = box;
		root->splitAxis = BSPAccelNode::X;

		root->refPrim = new Primitive*[pPrims->size()];
		root->nPrim = pPrims->size();

        for (int i=0;i<root->nPrim;i++) {
			root->refPrim[i] = pPrims->at(i);
        }
        
		_subdivide(root, 0);
	}

private:
    
    bool _hitObject(Ray r, BSPAccelNode* n, double tmin, double tmax, double light_dist);

    HitData _traversal(Ray r, BSPAccelNode* n, double tmin, double tmax);

	void _subdivide(BSPAccelNode *node, int depth);

};

#endif
