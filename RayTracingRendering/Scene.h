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
public:
    GLdouble _eyex;
    GLdouble _eyey;
    GLdouble _eyez;
    
    Light *_lights;
    Color _backgroundColor;
    
    Scene() {
        // default eye position
        _eyex = 0;
        _eyey = 0;
        _eyey = 10;
        
        _backgroundColor = { 0, 0, 0 };
    }
    
    Scene(const char* configFileName) {
        
    }
    
};



#endif /* defined(__RayTracingRendering__Scene__) */
