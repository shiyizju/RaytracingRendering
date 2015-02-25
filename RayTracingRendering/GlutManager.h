//
//  GlutManager.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/21/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__GlutManager__
#define __RayTracingRendering__GlutManager__

#include <stdio.h>
#include <OpenGL/gltypes.h>
#include <GLUT/GLUT.h>
#include <libkern/OSAtomic.h>

#include "Raytracer.h"

class GlutManager {
    
    GLint _w;
    GLint _h;
    
    GLubyte *_pixels;
    
    Raytracer *_rayTracer;
    
public:

    void initialize(int *argcp, char** argv, GLint width, GLint height, Raytracer* rayTracer) {
        _w = width;
        _h = height;
        _pixels = nullptr;
        _rayTracer = rayTracer;
        
        glutInit(argcp, argv);
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(width, height);
        glutInitWindowPosition(0, 0);
        glutCreateWindow(argv[0]);
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
    }
    
    void startLoop() {
        glutMainLoop();
    }
    
    static GlutManager& getInstance () {
        
        static GlutManager glutManager;
        return glutManager;
    }
    
private:
    
    GlutManager() { };
    GlutManager(GlutManager const&) = delete;
    
    
    static void display() {
        GlutManager::getInstance()._display();
    }
    
    static void reshape(int width, int height) {
        GlutManager::getInstance()._reshape(width, height);
    }
    
    static void mouse(int button, int state, int x, int y) {
        
    }
    
    void _display() {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear( GL_COLOR_BUFFER_BIT );
        glDrawPixels(_w, _h, GL_BGR, GL_UNSIGNED_BYTE, _pixels);
        glutSwapBuffers();
    }
    
    void _reshape(int width, int height) {
        
        _w = width;
        _h = height;
        
        if (_pixels) {
            delete []_pixels;
        }
        
        _pixels = new GLubyte[_w*_h*3];
        
        glViewport(0, 0, (GLsizei)_w, (GLsizei)_h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-10, 10, -10, 10, 10, 20);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 20, 0, 0, 0, 0, 1.0, 0.0f);
        
        GLdouble modelViewMatrix[16];
        GLdouble projectMatrix[16];
        GLint viewport[4];
        
        glGetDoublev(GL_MODELVIEW, modelViewMatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projectMatrix);
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        for (int j=0;j<_h;j++) {
            for (int i=0;i<_w;i++) {
                GLdouble x, y, z;
                gluUnProject((GLdouble)i, (GLdouble)j, 0.0, modelViewMatrix, projectMatrix, viewport, &x, &y, &z);
                Ray r(0, 0, 20, x, y, z);
                Color c = _rayTracer->trace(r);
                int pos = (j*_w + i)*3;
                _pixels[pos  ] = c.r;
                _pixels[pos+1] = c.g;
                _pixels[pos+2] = c.b;
            }
            std::cout<<(double)j/_h * 100<<"%"<<std::endl;
        }
    }
};

#endif /* defined(__RayTracingRendering__GlutManager__) */
