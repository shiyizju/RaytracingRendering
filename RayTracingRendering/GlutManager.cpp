//
//  GlutManager.cpp
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/21/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#include "GlutManager.h"

void GlutManager::initialize(int *argcp, char** argv, GLint width, GLint height, Raytracer* rayTracer) {
    
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

void GlutManager::_reshape(int width, int height) {
    
    _w = width;
    _h = height;
    
    if (_pixels) {
        delete []_pixels;
    }
    
    _pixels = new GLubyte[_w*_h*3];
    
    int eyex, eyey, eyez;
    _rayTracer->getEyePosition(eyex, eyey, eyez);
    
    glViewport(0, 0, (GLsizei)_w, (GLsizei)_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10, 10, -10, 10, 10, 20);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 1.0, 0.0f);
    
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
            Ray r(eyex, eyey, eyez, x, y, z);
            Color c = _rayTracer->trace(r);
            int pos = (j*_w + i)*3;
            _pixels[pos  ] = c.r;
            _pixels[pos+1] = c.g;
            _pixels[pos+2] = c.b;
        }
        std::cout<<(double)j/_h * 100<<"%"<<std::endl;
    }
}
