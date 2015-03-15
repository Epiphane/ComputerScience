//
//  coaster_car.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/14/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <iostream>
#include <glm/ext.hpp>
#include "coaster_car.h"
#include "bend.h"

#define COASTER_SPEED 0.05f

CoasterCar::CoasterCar() : nextCar(NULL), track(NULL), start(NULL) {
    this->load("models/car.obj");
    this->renderers[0]->mat = MATERIAL_RUBBER;
}

void CoasterCar::addCar(CoasterCar *car) {
    nextCar = car;
}

void CoasterCar::setStart(Track *_track) {
    start = track = _track;
    dist = 0;
}

void CoasterCar::update() {
    dist += COASTER_SPEED;
    
    while (track != NULL && dist >= 1.0f) {
        track = track->getNext();
        if (track == NULL)
            track = start;
        
        dist -= 1.0f;
    }
}

void CoasterCar::render() {
    glm::mat4 bend = glm::mat4(1.0f);
    if(track) {
        bend *= track->getModel();
        bend *= compute_bend(track->getBend(), dist);
    }
    
    std::vector<Renderer *>::iterator renderer;
    for (renderer = renderers.begin(); renderer != renderers.end(); renderer ++)
        (*renderer)->render(bend * Model);
    
    Renderer::pushMatrix(bend * Model);
    
    std::vector<Entity *>::iterator child;
    for (child = children.begin(); child != children.end(); child ++)
        (*child)->render();
    
    Renderer::popMatrix();
}