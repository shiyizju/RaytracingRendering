//
//  main.cpp
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#import <OpenGL/OpenGL.h>
#import <OpenGL/glu.h>
#import <OpenGL/gl.h>
#import <GLUT/GLUT.h>
#import <math.h>

#import "Raytracer.h"
#import "GlutManager.h"

using namespace std;

char scene_fn[64];
char output_fn[64];

Raytracer* raytracer;

int main(int argc, char** argv)
{
    cout<<"Input Scene File: "<<endl;
    cin>>scene_fn;
    strcat(scene_fn, ".sce");
    cout<<"Output File Name: "<<endl;
    cin>>output_fn;
    strcat(output_fn, ".jpg");
    
    raytracer = new Raytracer(scene_fn);
    
    GlutManager& glutManager = GlutManager::getInstance();
    glutManager.initialize(&argc, argv, 1000, 1000, raytracer);
    glutManager.startLoop();
    
    return 0;
}