//
//  world.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include "world.h"

World::World(const char *filename) : coasterCar(new Entity()), renderer(Program3D->create()) {
    coasterCar->load("models/bunny.obj");
    
    renderer->MVP = glm::translate(0, 0, -4);//glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0), glm::vec3(0, 1, 0));
    renderer->setView(glm::translate(0.0f, 0.0f, -4.0f));
}

void World::render() {
    renderer->pushMatrix();
    
    coasterCar->render();
    
    renderer->popMatrix();
}

void World::update() {
    
}