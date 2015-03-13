//
//  world.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include "world.h"
#include "camera.h"

#define MATH_PI 3.14159f

World::World(const char *filename) : coasterCar(new Entity()), track(new Entity()), renderer(Program3D->create()) {
    coasterCar->load("models/car.obj");
    coasterCar->getRenderer(0)->mat = MATERIAL_RUBBER;
    
    Entity *car2 = new Entity();
    car2->load("models/car.obj");
    car2->transform(glm::translate(0.0f, 0.0f, -1.0f));
    car2->getRenderer(0)->mat = MATERIAL_RUBBER;
    coasterCar->addChild(car2);
    
    track->load("models/track.obj");
    float sc = 2.6f;
    track->transform(glm::scale(sc, sc, sc));
    track->transform(glm::translate(0.0f, -0.13f, 0.0f));
    track->getRenderer(0)->bend.y = MATH_PI / 4;
    
    Entity *track2 = new Entity();
    track2->load("models/track.obj");
    track2->transform(glm::translate(-4 / MATH_PI, 0.0f, 0.0f));
    track2->transform(glm::rotate(180.0f / 4, 0.0f, -1.0f, 0.0f));
    track2->transform(glm::translate(4 / MATH_PI, 0.0f, 0.0f));
    track2->getRenderer(0)->bend.x = MATH_PI / 4;
    track->addChild(track2);
    
    Entity *track3 = new Entity();
    track3->load("models/track.obj");
    track3->transform(glm::translate(0.0f, 4 / MATH_PI, 0.0f));
    track3->transform(glm::rotate(180.0f / 4, -1.0f, 0.0f, 0.0f));
    track3->transform(glm::translate(0.0f, -4 / MATH_PI, 0.0f));
    track3->getRenderer(0)->bend.z = MATH_PI / 2;
    track2->addChild(track3);
    
    Entity *track4 = new Entity();
    track4->load("models/track.obj");
    track4->transform(glm::translate(0.0f, 0.0f, 0.0f));
    track4->transform(glm::rotate(180.0f / 2, 0.0f, 0.0f, -1.0f));
    track4->transform(glm::translate(0.0f, 0.0f, 1.0f));
    track4->getRenderer(0)->bend.x = MATH_PI / 2;
    track3->addChild(track4);
    
    Entity *track5 = new Entity();
    track5->load("models/track.obj");
    track5->transform(glm::translate(0.0f, 2 / MATH_PI, 0.0f));
    track5->transform(glm::rotate(180.0f / 2, -1.0f, 0.0f, 0.0f));
    track5->transform(glm::translate(0.0f, -2 / MATH_PI, 0.0f));
    track5->getRenderer(0)->bend.y = MATH_PI / 2;
    track4->addChild(track5);
    
    camera_setPosition(glm::vec3(2, 3, 4));
    camera_lookAt(glm::vec3(0, 0, 0));
}

void World::render() {
    coasterCar->render();
    
    track->render();
}

void World::update() {
    
}