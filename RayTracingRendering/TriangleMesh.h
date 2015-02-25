
#ifndef __RayTracingRendering__TRIANGLEMESH__
#define __RayTracingRendering__TRIANGLEMESH__

#include <math.h>
#include <vector>

#include "Basic.h"

class BufferFile
{
private:
	FILE *pFile;
	char *buf;
	int buf_len;
	int buf_idx;
	const int buf_size;

public:

	BufferFile(const char* filename, int bufSize = 1024) : buf_size(bufSize) {
		pFile = fopen(filename, "rb");
		buf = new char[buf_size];
		buf_len = 0;
		buf_idx = 0;
	}

	~BufferFile() {
		delete buf;
		fclose(pFile);
	}

	int getLine(char * line, int size) {
		int count = 0;
		while(count<size) {
            
            if (buf_idx==buf_len) {
                if (feof(pFile)) {
					break;
                }
                else {
					buf_len = fread (buf, 1, buf_size, pFile);
					buf_idx = 0;
				}
            }

			if (buf[buf_idx]=='\n') {
				buf_idx++;
				break;
			}
			
			line[count++] = buf[buf_idx++];
		}
		return count;
	}
    
	bool eof() {
		return feof(pFile) && (buf_idx==buf_len);
	}
};

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

	BufferFile file;

public:
	bool _isDigit(char c) {
		return c>='0'&&c<='9';
	}

	TriangleMesh(char* fn, double scale[3], double rot_angle[3], double trans[3]) : file(fn) {
		nface = 0;
		nvertex = 0;

		_parse(scale, rot_angle, trans);

		if (face_texture.size()==0)
			texted = false;
		if (face_norm.size()==0)
			normed = false;
	}

    void _parse(double scale[3], double rot_angle[3], double trans[3]);

};

#endif

