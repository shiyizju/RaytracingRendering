

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
	enum {
        X, Y, Z
    } splitAxis;
    
    // node type.
	enum {
        INNER, LEAF
    } type;
    
	AABB box;
    
	BSPAccelNode *left;
	BSPAccelNode *right;
	
    Primitive **refPrim;
	int nPrim;

	BSPAccelNode() : box(0,0,0,0,0,0) {
		left = NULL;
		right = NULL;
		refPrim = NULL;
		nPrim = 0;
	}
};

struct HitData
{
	Primitive *hitPrim;
	double t;
};

class Accelerator
{
public:
	BSPAccelNode* root;
	std::vector<Primitive*>* pPrims;

public:
	Accelerator(std::vector<Primitive*>* prim_vector)
	{
		pPrims = prim_vector;
		root = NULL;
	}

	bool hitObject(Ray r, double light_dist)
	{
		double tmin, tmax;
		root->box.intersect(r, tmin, tmax);

		if (tmin>tmax)
			return false;

		return _hitObject(r, root, tmin, tmax, light_dist);
	}
	
	HitData traversal(Ray r)
	{
		AABB box = root->box;

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

	void subdivide(AABB box)
	{
		root = new BSPAccelNode;
		root->box = box;
		root->splitAxis = BSPAccelNode::X;

		root->refPrim = new Primitive*[pPrims->size()];
		root->nPrim = pPrims->size();

		for (int i=0;i<root->nPrim;i++)
			root->refPrim[i] = pPrims->at(i);

		_subdivide(root, 0);
	}

private:
    
    bool _hitObject(Ray r, BSPAccelNode* n, double tmin, double tmax, double light_dist);

	HitData _traversal(Ray r, BSPAccelNode* n, double tmin, double tmax)
	{
		HitData hitd;
		hitd.hitPrim = NULL;
		hitd.t = DBL_MAX;

		if (n == NULL) return hitd;

		if (n->type == BSPAccelNode::LEAF) {
			for (int i=0;i<n->nPrim;i++) {
                
				double t = n->refPrim[i]->Intersection(r);
                
				if (t>-RAY_EPSILON && t< tmax+RAY_EPSILON && t<hitd.t) {
					hitd.t = t;
					hitd.hitPrim = n->refPrim[i];
				}
			}
			return hitd;
		}

		double tSplitPlane;

		BSPAccelNode *nearSide;
		BSPAccelNode *farSide;


		if (n->splitAxis == BSPAccelNode::X)
		{
			double half_x = (n->box.pmax.x + n->box.pmin.x)/2;

			if (double_equal(half_x, r.s.x) || double_equal(r.dir.x, 0))
				tSplitPlane = tmin;
			else
				tSplitPlane = ( half_x - r.s.x )/r.dir.x;


			if (double_equal(r.s.x, half_x))
			{
				if (r.dir.x>0)
				{
					nearSide = n->right;
					farSide = n->left;
				}
				else
				{
					nearSide = n->left;
					farSide = n->right;
				}
			}
			else if (r.s.x < half_x)
			{
				nearSide = n->left;
				farSide = n->right;
			}
			else
			{
				nearSide = n->right;
				farSide = n->left;
			}
		}
		else if (n->splitAxis == BSPAccelNode::Y)
		{
			double half_y = (n->box.pmin.y + n->box.pmax.y)/2;

			if (double_equal( half_y, r.s.y) || double_equal(r.dir.y, 0))
				tSplitPlane = tmin;
			else
				tSplitPlane = ( half_y - r.s.y )/r.dir.y;


			if (double_equal(r.s.y, half_y))
			{
				if (r.dir.y>0)
				{
					nearSide = n->right;
					farSide = n->left;
				}
				else
				{
					nearSide = n->left;
					farSide = n->right;
				}
			}		
			else if ( r.s.y < half_y)
			{
				nearSide = n->left;
				farSide = n->right;
			}
			else
			{
				nearSide = n->right;
				farSide = n->left;
			}
		}
		else
		{
			double half_z = (n->box.pmin.z + n->box.pmax.z)/2;

			if (double_equal(half_z, r.s.z) || double_equal(r.dir.z, 0))
				tSplitPlane = tmin;
			else
				tSplitPlane = ( half_z - r.s.z )/r.dir.z;

			if (double_equal(r.s.z, half_z)) {
                
				if (r.dir.z>0) {
					nearSide = n->right;
					farSide = n->left;
				}
				else {
					nearSide = n->left;
					farSide = n->right;
				}
			}
			else if ( r.s.z < half_z) {
				nearSide = n->left;
				farSide = n->right;
			}
			else {
				nearSide = n->right;
				farSide = n->left;
			}
		}

        if (double_larger(tSplitPlane, tmax) || double_smaller(tSplitPlane, 0)) {
			return _traversal(r, nearSide, tmin, tmax);
        }

        if (double_smaller(tSplitPlane, tmin)) {
			return _traversal(r, farSide, tmin, tmax);
        }
        
        if (double_equal(tSplitPlane, tmin)) {
			hitd = _traversal(r, nearSide, tmin, tmax);
        }
        else {
			hitd = _traversal(r, nearSide, tmin, tSplitPlane);
        }
        
		if (hitd.hitPrim!=NULL)
			return hitd;

		return _traversal(r, farSide, tSplitPlane, tmax);
	}

	void _subdivide(BSPAccelNode *node, int depth);

};

#endif
