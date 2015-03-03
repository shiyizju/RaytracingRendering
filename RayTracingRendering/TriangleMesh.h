
#ifndef __RayTracingRendering__TRIANGLEMESH__
#define __RayTracingRendering__TRIANGLEMESH__

#include <math.h>
#include <vector>

#include "Basic.h"
#include "BufferedLineReader.h"

class TriangleMesh
{
public:
	class TriangleIndex
	{
	public:
		int a, b, c;
	};

	int nvertex;
	int nface;

	std::vector<Point3D> vertex;
	std::vector<Direction> norm;
	std::vector<Point2D> text_cord;
	std::vector<TriangleIndex> face;
	std::vector<TriangleIndex> face_norm;
	std::vector<TriangleIndex> face_texture;

	bool texted;
	bool normed;

	BufferedLineReader file;

public:
	bool _isDigit(char c) {
		return c>='0'&&c<='9';
	}

	TriangleMesh(const char* fn, double scale[3], double rot_angle[3], double trans[3]) : file(fn) {
		nface = 0;
		nvertex = 0;

		_parse(scale, rot_angle, trans);

        if (face_texture.size()==0) {
			texted = false;
        }
        
        if (face_norm.size()==0) {
			normed = false;
        }
	}

    void _parse(double scale[3], double rot_angle[3], double trans[3]);

};

#endif

