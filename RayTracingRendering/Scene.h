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
#include <openGL/gltypes.h>

#include "Basic.h"

class Scene {
    
private:
    GLdouble _eyex;
    GLdouble _eyey;
    GLdouble _eyez;
    
    Light *_lights;
    Color _backgroundColor;
    
    friend class Raytracer;
    
public:
    Scene() {
        // default eye position
        _eyex = 15;
        _eyey = 0;
        _eyez = 10;
        
        _backgroundColor = { 0, 0, 0 };
    }
    
    // init scene from config file.
    Scene(const char* configFileName) {
        
    }
    
};



#endif /* defined(__RayTracingRendering__Scene__) */
