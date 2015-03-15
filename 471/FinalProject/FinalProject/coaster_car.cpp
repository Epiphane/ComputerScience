//
//  coaster_car.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/14/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include "coaster_car.h"

#define COASTER_SPEED 0.1f

CoasterCar::CoasterCar() : nextCar(NULL), track(NULL) {
    this->load("models/car.obj");
    this->renderers[0]->mat = MATERIAL_RUBBER;
}

void CoasterCar::addCar(CoasterCar *car) {
    nextCar = car;
}

void CoasterCar::update() {
    dist += COASTER_SPEED;
    
    while (dist >= 1.0f) {
        track = track->getNext();
        if (track == NULL)
            track = world->getTrack();
        
        dist -= 1.0f;
    }
}

void CoasterCar::render() {
}