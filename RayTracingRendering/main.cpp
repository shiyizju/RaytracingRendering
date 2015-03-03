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

#include "Scene.h"
#include "Raytracer.h"
#import "GlutManager.h"

using namespace std;

int main(int argc, char** argv)
{
    string sceneFolder;
    cout<<"Input scene folder: "<<endl;
    cin>>sceneFolder;
    
    string sceneFileName = PROJECT_DIR + sceneFolder + "/";
    
    Scene scene(sceneFileName.c_str());
    
    Raytracer* raytracer = new Raytracer(&scene);
    
    GlutManager& glutManager = GlutManager::getInstance();
    glutManager.initialize(&argc, argv, 1000, 1000, raytracer);
    glutManager.startLoop();
    
    return 0;
}