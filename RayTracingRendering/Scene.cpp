//
//  Scene.cpp
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/22/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#include "Scene.h"

#include "Shape.h"
#include "Rect3D.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Primitive.h"

Scene::Scene() {
    // Default eye position.
    _eyex = 15;
    _eyey = 0;
    _eyez = 10;
    
    // Default ambient light.
    _ambientLight.r = 0.1;
    _ambientLight.g = 0.1;
    _ambientLight.b = 0.1;
    
    _backgroundColor = { 0, 0, 0 };
}

Scene::Scene(const char* sceneDir) {
    
    std::string sceneFolder = sceneDir;
    
    // Default eye position.
    _eyex = 0;
    _eyey = 0;
    _eyez = 20;
    
    // Default ambient light.
    _ambientLight.r = 0.1;
    _ambientLight.g = 0.1;
    _ambientLight.b = 0.1;
    
    _backgroundColor = { 0, 0, 0 };
    
    // Configuration from file.
    std::string sceneFileName = sceneFolder + "scene";
    
    FILE *file = fopen(sceneFileName.c_str(), "rb");
    
    char buf[256];
    
    while (!feof(file)) {
        
        fscanf(file, "%s", buf);
        
        if (strcmp(buf, "eye") == 0) {
            
            fscanf(file, "%lf %lf %lf", &_eyex, &_eyey, &_eyez);
            
        }
        else if (strcmp(buf, "object") == 0) {
            
            fscanf(file, "%s", buf);
            std::string objFileName = sceneFolder + buf;
            
            double scale[3], rotation[3], transform[3];
            
            fscanf(file, "%lf %lf %lf", scale,     scale+1,     scale+2);
            fscanf(file, "%lf %lf %lf", rotation,  rotation+1,  rotation+2);
            fscanf(file, "%lf %lf %lf", transform, transform+1, transform+2);
            
            TriangleMesh* tm = new TriangleMesh(objFileName.c_str(), scale, rotation, transform);

            Material* m = new Material;
            Texture* pTexture = NULL;
            bool texted;
            
            fscanf(file, "%s", buf);
            
            if (strcmp(buf, "texture") == 0) {
                texted = true;
                fscanf(file, "%s", buf);
                std::string textPath = sceneFolder + buf;
                pTexture = new Texture(textPath.c_str());
            }
            else {
                texted = false;
                _readMaterial(file, *m);
            }
            
            for (int i=0;i<tm->nface;i++) {
                Shape* triangle = new Triangle(tm, i);
                _primitive.push_back(new Primitive(triangle, m, texted, pTexture));
            }
        }
        else if (strcmp(buf, "sphere") == 0) {
            // Sphere
            Point3D center;
            fscanf(file, "%lf %lf %lf", &center.x, &center.y, &center.z);
            double radius;
            fscanf(file, "%lf", &radius);
            
            Shape *s = new Sphere(center, radius);
            
            // Materail
            Material* m = new Material;
            _readMaterial(file, *m);
            
            _primitive.push_back(new Primitive(s, m));
        }
        else if (strcmp(buf, "box") == 0) {
            // Box
            Point3D p1, p2;
            fscanf(file, "%lf %lf %lf", &p1.x, &p1.y, &p1.z);
            fscanf(file, "%lf %lf %lf", &p2.x, &p2.y, &p2.z);
            Shape *s = new Rect3D(p1, p2);
            
            // Material
            Material* m = new Material;
            _readMaterial(file, *m);

            _primitive.push_back(new Primitive(s, m));
        }
        else if (strcmp(buf, "light") == 0) {
            Light light;
            
            fscanf(file, "%lf %lf %lf", &light.r, &light.g, &light.b);
            fscanf(file, "%lf %lf %lf", &light.p.x, &light.p.y, &light.p.z);
            
            _pointLight.push_back(light);
        }
    }
    fclose(file);
}

void Scene::_readMaterial(FILE *file, Material &material) {
    
    // For the meaning of the material parameters, refert to http://en.wikipedia.org/wiki/Phong_reflection_model
    
    fscanf(file, "%lf %lf %lf", &material.rKa, &material.rKd, &material.rKs);
    fscanf(file, "%lf %lf %lf", &material.gKa, &material.gKd, &material.gKs);
    fscanf(file, "%lf %lf %lf", &material.bKa, &material.bKd, &material.bKs);
    fscanf(file, "%d %lf", (int*)&material.reflective, &material.refraction_index);
    
}





