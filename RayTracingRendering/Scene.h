//
//  Scene.h
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/22/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#ifndef __RayTracingRendering__Scene__
#define __RayTracingRendering__Scene__

#include <stdio.h>
#include <vector>
#include <openGL/gltypes.h>

#include "Basic.h"

class Sphere;
class Primitive;

class Scene {
    
    friend class Raytracer;

private:
    // eye position.
    GLdouble _eyex;
    GLdouble _eyey;
    GLdouble _eyez;
    
    // ambient light
    Light _ambientLight;
    
    // Point light
    std::vector<Light> _pointLight;
    
    std::vector<Primitive*> _primitive;
    
    Color _backgroundColor;
    
public:
    
    Scene();
    
    Scene(const char* sceneDir);
    
    
private:
        
    void _readMaterial(FILE* file, Material& material);
        
};



#endif /* defined(__RayTracingRendering__Scene__) */
