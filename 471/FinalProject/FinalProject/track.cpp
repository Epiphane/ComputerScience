//
//  track.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/12/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <iostream>
#include <string>
#include "track.h"
#include "main.h"

std::vector<tinyobj::shape_t> track_shapes;
std::vector<tinyobj::material_t> track_materials;
std::vector<float> posBuf, norBuf;
std::vector<unsigned int> indBuf;
bool initialized = false;

Track::Track() {
    if (!initialized) {
        std::string err = tinyobj::LoadObj(track_shapes, track_materials, "models/track.obj");
        if(!err.empty()) {
            std::cerr << err << std::endl;
        }
        resize_obj(track_shapes);
        
        std::cout << track_shapes.size() << std::endl;
        for(int s = 0; s < track_shapes.size(); s ++) {
            posBuf = track_shapes[s].mesh.positions;
            
            norBuf.clear();
            int idx1, idx2, idx3;
            glm::vec3 v1, v2, v3;
            //for every vertex initialize a normal to 0
            for (int j = 0; j < track_shapes[s].mesh.positions.size()/3; j++) {
                norBuf.push_back(0);
                norBuf.push_back(0);
                norBuf.push_back(0);
            }
            // DO work here to compute the normals for every face
            //then add its normal to its associated vertex
            for (int i = 0; i < track_shapes[s].mesh.indices.size()/3; i++) {
                idx1 = track_shapes[s].mesh.indices[3*i+0];
                idx2 = track_shapes[s].mesh.indices[3*i+1];
                idx3 = track_shapes[s].mesh.indices[3*i+2];
                v1 = glm::vec3(track_shapes[s].mesh.positions[3*idx1 +0],track_shapes[s].mesh.positions[3*idx1 +1], track_shapes[s].mesh.positions[3*idx1 +2]);
                v2 = glm::vec3(track_shapes[s].mesh.positions[3*idx2 +0],track_shapes[s].mesh.positions[3*idx2 +1], track_shapes[s].mesh.positions[3*idx2 +2]);
                v3 = glm::vec3(track_shapes[s].mesh.positions[3*idx3 +0],track_shapes[s].mesh.positions[3*idx3 +1], track_shapes[s].mesh.positions[3*idx3 +2]);
                
                glm::vec3 u, v;
                
                u = v2 - v1;
                v = v3 - v1;
                norBuf[3*idx1+0] += u.y * v.z - u.z * v.y;
                norBuf[3*idx1+1] += u.z * v.x - u.x * v.z;
                norBuf[3*idx1+2] += u.x * v.y - u.y * v.x;
                
                u = v3 - v2;
                v = v1 - v2;
                norBuf[3*idx2+0] += u.y * v.z - u.z * v.y;
                norBuf[3*idx2+1] += u.z * v.x - u.x * v.z;
                norBuf[3*idx2+2] += u.x * v.y - u.y * v.x;
                
                u = v1 - v3;
                v = v2 - v3;
                norBuf[3*idx3+0] += u.y * v.z - u.z * v.y;
                norBuf[3*idx3+1] += u.z * v.x - u.x * v.z;
                norBuf[3*idx3+2] += u.x * v.y - u.y * v.x;
            }
        }
    }
    
    Renderer *renderer = Program3D->create();
    
    renderer->setNumElements(indBuf.size());
    renderer->bufferData(INDICES_BUFFER, indBuf.size(), (void *)&indBuf[0]);
    
    renderer->bufferData(VERTEX_BUFFER, posBuf.size(), (void *)&posBuf[0]);
    renderer->bufferData(NORMAL_BUFFER, norBuf.size(), (void *)&norBuf[0]);
    
    renderers.push_back(renderer);
}