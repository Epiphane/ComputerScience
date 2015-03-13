////
////  track.cpp
////  FinalProject
////
////  Created by Thomas Steinke on 3/12/15.
////  Copyright (c) 2015 Thomas Steinke. All rights reserved.
////
//
//#include <iostream>
//#include <string>
//#include "track.h"
//#include "main.h"
//
//std::vector<tinyobj::shape_t> track_shapes;
//std::vector<tinyobj::material_t> track_materials;
//std::vector<float> posBuf, norBuf;
//std::vector<unsigned int> indBuf;
//bool initialized = false;
//
//Track::Track() : nextTrack(NULL) {
//    if (!initialized) {
//        std::string err = tinyobj::LoadObj(track_shapes, track_materials, "models/track.obj");
//        if(!err.empty()) {
//            std::cerr << err << std::endl;
//        }
//        resize_obj(track_shapes);
//        
//        std::cout << track_shapes.size() << std::endl;
//        for(int s = 0; s < track_shapes.size(); s ++) {
//            posBuf = track_shapes[s].mesh.positions;
//            
//            norBuf.clear();
//            int idx1, idx2, idx3;
//            glm::vec3 v1, v2, v3;
//            //for every vertex initialize a normal to 0
//            for (int j = 0; j < track_shapes[s].mesh.positions.size()/3; j++) {
//                norBuf.push_back(0);
//                norBuf.push_back(0);
//                norBuf.push_back(0);
//            }
//            // DO work here to compute the normals for every face
//            //then add its normal to its associated vertex
//            for (int i = 0; i < track_shapes[s].mesh.indices.size()/3; i++) {
//                idx1 = track_shapes[s].mesh.indices[3*i+0];
//                idx2 = track_shapes[s].mesh.indices[3*i+1];
//                idx3 = track_shapes[s].mesh.indices[3*i+2];
//                v1 = glm::vec3(track_shapes[s].mesh.positions[3*idx1 +0],track_shapes[s].mesh.positions[3*idx1 +1], track_shapes[s].mesh.positions[3*idx1 +2]);
//                v2 = glm::vec3(track_shapes[s].mesh.positions[3*idx2 +0],track_shapes[s].mesh.positions[3*idx2 +1], track_shapes[s].mesh.positions[3*idx2 +2]);
//                v3 = glm::vec3(track_shapes[s].mesh.positions[3*idx3 +0],track_shapes[s].mesh.positions[3*idx3 +1], track_shapes[s].mesh.positions[3*idx3 +2]);
//                
//                glm::vec3 u, v;
//                
//                u = v2 - v1;
//                v = v3 - v1;
//                norBuf[3*idx1+0] += u.y * v.z - u.z * v.y;
//                norBuf[3*idx1+1] += u.z * v.x - u.x * v.z;
//                norBuf[3*idx1+2] += u.x * v.y - u.y * v.x;
//                
//                u = v3 - v2;
//                v = v1 - v2;
//                norBuf[3*idx2+0] += u.y * v.z - u.z * v.y;
//                norBuf[3*idx2+1] += u.z * v.x - u.x * v.z;
//                norBuf[3*idx2+2] += u.x * v.y - u.y * v.x;
//                
//                u = v1 - v3;
//                v = v2 - v3;
//                norBuf[3*idx3+0] += u.y * v.z - u.z * v.y;
//                norBuf[3*idx3+1] += u.z * v.x - u.x * v.z;
//                norBuf[3*idx3+2] += u.x * v.y - u.y * v.x;
//            }
//        }
//    }
//    
//    Renderer *renderer = Program3D->create();
//    
//    renderer->setNumElements(indBuf.size());
//    renderer->bufferData(INDICES_BUFFER, indBuf.size(), (void *)&indBuf[0]);
//    
//    renderer->bufferData(VERTEX_BUFFER, posBuf.size(), (void *)&posBuf[0]);
//    renderer->bufferData(NORMAL_BUFFER, norBuf.size(), (void *)&norBuf[0]);
//    
//    renderers.push_back(renderer);
//}
//
//void Track::bend(glm::vec3 bend) {
//    this->renderers[0]->bend = bend;
//}
//
//void Track::setNext(Track *child) {
//    this->nextTrack = child;
//}
//
//void Track::render() {
//    Entity::render();
//    
//    // Matrix is still pushed, now draw next track!
//    if (this->nextTrack) {
//        glm::vec3 bend = this->renderers[0]->bend;
//        glm::mat4 model = model
//        
//        if (bend.y) {
//            
//        }
//    }
//    
//        
//    track->load("models/track.obj");
//    float sc = 2.6f;
//    track->transform(glm::scale(sc, sc, sc));
//    track->transform(glm::translate(0.0f, -0.13f, 0.0f));
//    track->getRenderer(0)->bend.y = MATH_PI / 4;
//    
//    Entity *track2 = new Entity();
//    track2->load("models/track.obj");
//    track2->transform(glm::translate(-4 / MATH_PI, 0.0f, 0.0f));
//    track2->transform(glm::rotate(180.0f / 4, 0.0f, -1.0f, 0.0f));
//    track2->transform(glm::translate(4 / MATH_PI, 0.0f, 0.0f));
//    track2->getRenderer(0)->bend.x = MATH_PI / 4;
//    track->addChild(track2);
//    
//    Entity *track3 = new Entity();
//    track3->load("models/track.obj");
//    track3->transform(glm::translate(0.0f, 4 / MATH_PI, 0.0f));
//    track3->transform(glm::rotate(180.0f / 4, -1.0f, 0.0f, 0.0f));
//    track3->transform(glm::translate(0.0f, -4 / MATH_PI, 0.0f));
//    track3->getRenderer(0)->bend.z = MATH_PI / 2;
//    track2->addChild(track3);
//    
//    Entity *track4 = new Entity();
//    track4->load("models/track.obj");
//    track4->transform(glm::translate(0.0f, 0.0f, 0.0f));
//    track4->transform(glm::rotate(180.0f / 2, 0.0f, 0.0f, -1.0f));
//    track4->transform(glm::translate(0.0f, 0.0f, 1.0f));
//    track4->getRenderer(0)->bend.x = MATH_PI / 2;
//    track3->addChild(track4);
//    
//    Entity *track5 = new Entity();
//    track5->load("models/track.obj");
//    track5->transform(glm::translate(0.0f, 2 / MATH_PI, 0.0f));
//    track5->transform(glm::rotate(180.0f / 2, -1.0f, 0.0f, 0.0f));
//    track5->transform(glm::translate(0.0f, -2 / MATH_PI, 0.0f));
//    track5->getRenderer(0)->bend.y = MATH_PI / 2;
//    track4->addChild(track5);
//}