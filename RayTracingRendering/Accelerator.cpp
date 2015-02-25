
#include "accelerator.h"


const int MAX_TREEDEPTH = 30;
const int PRIM_LIMIT = 20;


void Accelerator::_subdivide(BSPAccelNode *node, int depth)
{
	if (node->nPrim <= PRIM_LIMIT||depth > MAX_TREEDEPTH) {
		node->type = BSPAccelNode::LEAF;
		return;
	}

	node->type = BSPAccelNode::INNER;
	node->left = new BSPAccelNode;
	node->right = new BSPAccelNode;

	BSPAccelNode *left = node->left, *right = node->right;

	left->box = node->box;
	right->box = node->box;

	left->refPrim = new Primitive*[node->nPrim];
	right->refPrim = new Primitive*[node->nPrim];
	//now split
	if (node->splitAxis == BSPAccelNode::X) {
        
		left->splitAxis = BSPAccelNode::Y;
		right->splitAxis = BSPAccelNode::Y;

		double mid = (node->box.pmin.x + node->box.pmax.x)/2;
		left->box.pmax.x = mid;
		right->box.pmin.x = mid;
	}
	else if (node->splitAxis == BSPAccelNode::Y) {
        
		left->splitAxis = BSPAccelNode::Z;
		right->splitAxis = BSPAccelNode::Z;

		double mid = (node->box.pmin.y + node->box.pmax.y)/2;
		left->box.pmax.y = mid;
		right->box.pmin.y = mid;
	}
	else if (node->splitAxis == BSPAccelNode::Z) {
        
		left->splitAxis = BSPAccelNode::X;
		right->splitAxis = BSPAccelNode::X;

		double mid = (node->box.pmin.z + node->box.pmax.z)/2;
		left->box.pmax.z = mid;
		right->box.pmin.z = mid;
	}

	for (int i=0;i<node->nPrim;i++) {
        
		AABB objBoundingBox = node->refPrim[i]->BoundingBox();
        if (left->box.overlaps(objBoundingBox)) {
			left->refPrim[left->nPrim++] = node->refPrim[i];
        }
        
        if (right->box.overlaps(objBoundingBox)){
			right->refPrim[right->nPrim++] = node->refPrim[i];
        }
	}

	delete node->refPrim;   //this field is useless for inner node
	node->refPrim = NULL;

	_subdivide(left, depth+1);
	_subdivide(right, depth+1);
}


bool Accelerator::_hitObject(Ray r, BSPAccelNode* n, double tmin, double tmax, double light_dist)
{
    if (n == NULL) return false;
    
    if (n->type == BSPAccelNode::LEAF)
    {
        for (int i=0;i<n->nPrim;i++)
        {
            double t = n->refPrim[i]->Intersection(r);
            if (t>0 && t<tmax + RAY_EPSILON && t<light_dist)
                return true;
        }
        return false;
    }
    
    double tSplitPlane;
    
    BSPAccelNode *nearSide;
    BSPAccelNode *farSide;
    
    
    if (n->splitAxis == BSPAccelNode::X)
    {
        double half_x = (n->box.pmax.x + n->box.pmin.x)/2;
        
        if (double_equal(half_x, r.s.x) || double_equal(r.dir.x, 0)) {
            tSplitPlane = tmin;
        }
        else {
            tSplitPlane = ( half_x - r.s.x )/r.dir.x;
        }
        
        if (double_equal(r.s.x, half_x)) {
            
            if (r.dir.x>0) {
                nearSide = n->right;
                farSide = n->left;
            }
            else {
                nearSide = n->left;
                farSide = n->right;
            }
        }
        else if (r.s.x < half_x) {
            nearSide = n->left;
            farSide = n->right;
        }
        else {
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
        
        
        if (double_equal(r.s.y, half_y)) {
            if (r.dir.y>0) {
                nearSide = n->right;
                farSide = n->left;
            }
            else {
                nearSide = n->left;
                farSide = n->right;
            }
        }
        else if ( r.s.y < half_y) {
            nearSide = n->left;
            farSide = n->right;
        }
        else {
            nearSide = n->right;
            farSide = n->left;
        }
    }
    else {
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
        return _hitObject(r, nearSide, tmin, tmax, light_dist);
    }
    
    if (double_smaller(tSplitPlane, tmin)) {
        return _hitObject(r, farSide, tmin, tmax, light_dist);
    }
    
    bool ans;
    if (double_equal(tSplitPlane, tmin))
        ans = _hitObject(r, nearSide, tmin, tmax, light_dist);
    else
        ans = _hitObject(r, nearSide, tmin, tSplitPlane, light_dist);
    if (ans)
        return true;
    
    return _hitObject(r, farSide, tSplitPlane, tmax, light_dist);
}





