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
//#include <libkern/OSAtomic.h>

#include "Raytracer.h"

class GlutManager {
    
private:
    
    GLint _w;
    GLint _h;
    
    GLubyte *_pixels;
    
    Raytracer *_rayTracer;

public:

    void initialize(int *argcp, char** argv, GLint width, GLint height, Raytracer* rayTracer);
    
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
    
    void _reshape(int width, int height);
};

#endif /* defined(__RayTracingRendering__GlutManager__) */
