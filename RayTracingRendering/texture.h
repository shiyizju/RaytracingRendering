
#import "basic.h"


//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cxcore.lib")

class Texture
{
public:
	char tx_name[128];
	//IplImage* img;
	int w;
	int h;

	Texture (const char* filename)
	{/*
		img = cvLoadImage(filename);
		std::cout<<(img==NULL)<<std::endl;
		w = img->width;
		h = img->height;
		strcpy(tx_name, filename);*/
	}
	Color getTexture(const Point2D& p)
	{
		Color c;
//		double px = p.x - floor(p.x);
//		double py = p.y - floor(p.y);
//		int x = px*w;
//		int y = (1-py)*(h-1);
//
//		c.r = img->imageData[(x+y*w)*3];
//		c.g = img->imageData[(x+y*w)*3+1];
//		c.b = img->imageData[(x+y*w)*3+2];

		return c;
	}
};