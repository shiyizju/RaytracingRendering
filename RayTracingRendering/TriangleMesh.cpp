//
//  TriangleMesh.cpp
//  RayTracingRendering
//
//  Created by Xiaosha Quan on 2/8/15.
//  Copyright (c) 2015 QUAN. All rights reserved.
//

#import "TriangleMesh.h"

void TriangleMesh::_parse(double scale[3], double rot_angle[3], double trans[3]) {
    
    char line[1024];
    
    while(!file.eof())
    {
        int count = file.getLine(line, 1024);
        
        line[count]=0;
        
        if (line[0]=='v'&&line[1]==' ')
        {
            Point3D p;
            int i=1;
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &p.x);
            while (line[i]!=' ') i++;
            
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &p.y);
            while (line[i]!=' ') i++;
            
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &p.z);
            
            double x = p.x*scale[0],
            y = p.y*scale[1],
            z = p.z*scale[2];
            
            double tx, ty, tz;
            double vcos, vsin;
            
            vcos = cos(rot_angle[0]);
            vsin = sin(rot_angle[0]);
            ty = vcos*y - vsin*z;
            tz = vsin*y + vcos*z;
            tx = x;
            x = tx; y = ty; z = tz;
            
            vcos = cos(rot_angle[1]);
            vsin = sin(rot_angle[1]);
            tz = vcos*z - vsin*x;
            tx = vsin*z + vcos*x;
            ty = y;
            x = tx; y = ty; z = tz;
            
            vcos = cos(rot_angle[2]);
            vsin = sin(rot_angle[2]);
            tx = vcos*x - vsin*y;
            ty = vsin*x + vcos*y;
            tz = z;
            x = tx; y = ty; z = tz;
            
            p.x = x+trans[0]; p.y = y+trans[1]; p.z = z+trans[2];
            vertex.push_back(p);
            nvertex++;
        }
        else if(line[0]=='v'&&line[1]=='n')
        {
            double x, y, z;
            int i=2;
            
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &x);
            while (line[i]!=' ') i++;
            
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &y);
            while (line[i]!=' ') i++;
            
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &z);
            
            
            x *= scale[0];
            y *= scale[1];
            z *= scale[2];
            
            double tx, ty, tz;
            double vcos, vsin;
            
            vcos = cos(rot_angle[0]);
            vsin = sin(rot_angle[0]);
            ty = vcos*y - vsin*z;
            tz = vsin*y + vcos*z;
            tx = x;
            x = tx; y = ty; z = tz;
            
            vcos = cos(rot_angle[1]);
            vsin = sin(rot_angle[1]);
            tz = vcos*z - vsin*x;
            tx = vsin*z + vcos*x;
            ty = y;
            x = tx; y = ty; z = tz;
            
            vcos = cos(rot_angle[2]);
            vsin = sin(rot_angle[2]);
            tx = vcos*x - vsin*y;
            ty = vsin*x + vcos*y;
            tz = z;
            x = tx; y = ty; z = tz;
            
            Direction d(x, y, z);
            norm.push_back(d);
        }
        else if (line[0]=='v'&&line[1]=='t')
        {
            Point2D p2d;
            int i=2;
            
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &p2d.x);
            while (line[i]!=' ') i++;
            
            while (line[i]==' ') i++;
            sscanf(&line[i], "%lf", &p2d.y);
            
            text_cord.push_back(p2d);
        }
        else if(line[0]=='f')
        {
            int v[3] =  {0, 0, 0};
            int vt[3] = {0, 0, 0};
            int vn[3] = {0, 0, 0};
            
            int i=1;
            
            for (int n=0;n<3;n++)
            {
                while (!_isDigit(line[i])) i++;
                sscanf(&line[i], "%d", &v[n]);
                while (_isDigit(line[i])) i++;
                if(line[i]=='/'&&line[i+1]!='/')
                {
                    sscanf(&line[++i], "%d", &vt[n]);
                    while(_isDigit(line[i])) i++;
                    if (line[i]=='/')
                    {
                        sscanf(&line[++i], "%d", &vn[n]);
                        while(_isDigit(line[i])) i++;
                    }
                }
                else if(line[i]=='/'&&line[i+1]=='/')
                {
                    i+=2;
                    sscanf(&line[i], "%d", &vn[n]);
                    while (_isDigit(line[i])) i++;
                }
            }
            
            for (int i=0;i<3;i++)
            {
                v[i]--;
                vt[i]--;
                vn[i]--;
            }
            
            TriangleIndex ti;
            ti.a = v[0];
            ti.b = v[1];
            ti.c = v[2];
            face.push_back(ti);
            nface++;
            
            if (vt[0]!=-1)
            {
                ti.a = vt[0];
                ti.b = vt[1];
                ti.c = vt[2];
                face_texture.push_back(ti);
            }
            
            if (vn[0]!=-1)
            {
                ti.a = vn[0];
                ti.b = vn[1];
                ti.c = vn[2];
                face_norm.push_back(ti);
            }
        }
    }
}