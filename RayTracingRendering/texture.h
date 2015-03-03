//
//  Texture.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 3/1/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef RayTracingRendering_Texture_h
#define RayTracingRendering_Texture_h

#include <stdio.h>
#include <stdlib.h>

#include "Basic.h"
#include "png.h"


class Texture {
    
    unsigned int _w;
    unsigned int _h;
    
    unsigned char **pixels;
    
public:
    
    Texture (const char* filename) {
        
        unsigned char header[8];    // 8 is the maximum size that can be checked
        
        /* open file and test for it being a png */
        FILE *fp = fopen(filename, "rb");
        if (!fp) {
            return;
        }
        
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8)) {
            return;
        }
        
        /* initialize stuff */
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        
        if (!png_ptr) {
            return;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            return;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            return;
        }
        
        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);
        
        png_read_info(png_ptr, info_ptr);
        
        _w = png_get_image_width(png_ptr, info_ptr);
        _h = png_get_image_height(png_ptr, info_ptr);
        
        png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        
        int number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);
        
        /* read file */
        if (setjmp(png_jmpbuf(png_ptr))) {
            return;
        }
        
        pixels = (unsigned char**) malloc( _h * sizeof(unsigned char*) );
        for (int y=0; y<_h; y++) {
            pixels[y] = (unsigned char*) malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
        
        png_read_image(png_ptr, pixels);
        
        fclose(fp);
        
    }
    
    Color getTexture(const Point2D& p) {
        
        Color c;
        double px = p.x - floor(p.x);
        double py = p.y - floor(p.y);
        int x = px * _w;
        int y = (1 - py) * (_h - 1);

        c.r = pixels[y][x*3];
        c.g = pixels[y][x*3+1];
        c.b = pixels[y][x*3+2];
        
        return c;
    }
};


#endif
