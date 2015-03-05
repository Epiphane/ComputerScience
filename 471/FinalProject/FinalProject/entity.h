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
    std::vector<Renderer *> renderers;
    
    unsigned int posBufObj, norBufObj, indBufObj;
public:
    Entity();
    
    void load(const char *filename);
    
    void update();
    void render();
};

#endif /* defined(__FinalProject__entity__) */
