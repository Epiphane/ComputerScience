//
//  entity.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__entity__
#define __FinalProject__entity__

#include "tiny_obj_loader.h"
#include "renderer.h"

class Entity {
private:
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    unsigned int posBufObj, norBufObj, indBufObj;
    glm::mat4 Model;
    
protected:
    std::vector<Renderer *> renderers;
    std::vector<Entity *> children;
    
public:
    Entity();
    
    void load(const char *filename);
    
    void addChild(Entity *e);
    void transform(glm::mat4 mat);
    void update();
    void render();
    
    Renderer *getRenderer(int num);
};

#endif /* defined(__FinalProject__entity__) */
