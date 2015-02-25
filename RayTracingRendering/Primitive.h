
#ifndef __RT_PRIMITIVE_H
#define __RT_PRIMITIVE_H

#include "Basic.h"
#include "Shape.h"
#include "Texture.h"
#include "AABB.h"

#include <iostream>

class Primitive
{
public:
	Material* m;
	Shape* s;
	bool textured;
	Texture *pTx;

	Primitive(Shape *shape, Material* material, bool textured=false, Texture *pTexture=NULL)
	{
		s = shape;
		m = material;
		this->textured = textured;
		pTx = pTexture;
	}

	double Intersection(Ray r)
	{
		return s->intersection(r);
	};

	Color getTexture(Point3D p)
	{
		Point2D tx_cord = s->getTextureCoord(p);
		return pTx->getTexture(tx_cord);
	}

	AABB BoundingBox()
	{
		return s->boundingBox();
	}
};

#endif