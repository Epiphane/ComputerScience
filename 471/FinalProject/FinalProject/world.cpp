//
//  world.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include "world.h"

World::World(const char *filename) : coasterCar(new Entity()), track(new Entity()), renderer(Program3D->create()) {
//    coasterCar->load("models/car.obj");
//    coasterCar->getRenderer(0)->mat = MATERIAL_RUBBER;
    
//    Entity *car2 = new Entity();
//    car2->load("models/car.obj");
//    car2->transform(glm::translate(0.0f, 0.0f, -2.0f));
//    car2->getRenderer(0)->mat = MATERIAL_RUBBER;
//    coasterCar->addChild(car2);
    
//    track->load("models/track.obj");
//    track->transform(glm::translate(-2.0f, 0.0f, 0.0f));
    
//    renderer->setView(glm::lookAt(glm::vec3(3, 3, 4), glm::vec3(0), glm::vec3(0, 1, 0)));
}

void World::render() {
//    coasterCar->render();
//    
//    track->render();
}

void World::update() {
    
}