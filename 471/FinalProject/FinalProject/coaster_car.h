//
//  coaster_car.h
//  FinalProject
//
//  Created by Thomas Steinke on 3/14/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#ifndef __FinalProject__coaster_car__
#define __FinalProject__coaster_car__

#include "entity.h"
#include "world.h"
#include "track.h"

class CoasterCar : public Entity {
private:
    World *world;
    
    CoasterCar *nextCar;
    Track *track;
    float dist;

public:
    CoasterCar();
    
    void setWorld(World *_world) { world = _world; }
    void setTrack(Track *_track) { track = _track; dist = 0; }
    void addCar(CoasterCar *car);
    
    void update();
    void render();
};

#endif /* defined(__FinalProject__coaster_car__) */
