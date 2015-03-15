//
//  world.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/3/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include "world.h"
#include "camera.h"
#include "main.h"
#include "coaster_car.h"

World::World(const char *filename) : car(new CoasterCar()), track(new Track()), renderer(Program3D->create()) {
    // Move camera
    camera_setPosition(glm::vec3(2, 3, 4));
    camera_lookAt(glm::vec3(0, 0, 0));
    
    // Create initial track piece
    float track_sc = 2.6f;
    track->transformBefore(glm::scale(track_sc, track_sc, track_sc));
    track->transformBefore(glm::translate(0.0f, -0.13f, 0.0f));
    
    Track *track = this->track;
    Track *next;
    
    // Load track from file
    std::ifstream trackFile(filename);
    glm::vec3 bend;
    std::string line;
    while (std::getline(trackFile, line)) {
        std::istringstream iss(line);
        if (iss.str().find("//") != std::string::npos ||
            iss.str().length() == 0) {
            // Ignore ...
        }
        else if (iss >> bend.x >> bend.y >> bend.z) {
            // Track input is degrees, convert to radians
            bend *= DEG_TO_RADIANS;
            
            next = new Track();
            next->bend(bend);
            
            track->setNext(next);
            track = next;
        }
        else {
            std::cerr << "Improper track line: " << iss.str() << std::endl;
        }
    }
    
    while (trackFile >> bend.x >> bend.y >> bend.z) {
        // Track input is degrees, convert to radians
        bend *= DEG_TO_RADIANS;
        
        next = new Track();
        next->bend(bend);
        
        track->setNext(next);
        track = next;
    }
    
    CoasterCar *c = static_cast<CoasterCar *>(car);
    c->setTrack(this->track);
    c->addCar(new CoasterCar());
    c->setWorld(this);
}

void World::render() {
    car->render();
    
    track->render();
}

void World::update() {
    car->update();
}