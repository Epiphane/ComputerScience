//
//  world.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__world__
#define __FinalProject__world__

#include <stdio.h>
#include "entity.h"

class World {
private:
    Entity *coasterCar;
    Entity *track;
    Renderer *renderer;
    
    glm::vec2 cameraDir;
    
public:
    World(const char *filename);
    
    void update();
    void render();
};

#endif /* defined(__FinalProject__world__) */
