//
//  camera.cpp
//  FinalProject
//
//  Created by Thomas Steinke on 3/11/15.
//  Copyright (c) 2015 Thomas Steinke. All rights reserved.
//

#include <glm/ext.hpp>
#include "main.h"
#include "camera.h"

glm::vec3 position;
double pitch, yaw;

void camera_setPosition(glm::vec3 _position) {
    position = _position;
}

void camera_lookAt(glm::vec3 dest) {
    glm::vec3 dir = glm::normalize(dest - position);
    
    pitch = asin(dir.y);
    yaw = acos(dir.x / cos(pitch));
}

void camera_setPitch(double _pitch) {
    pitch = _pitch;
    if (pitch > MATH_PI / 2)
        pitch = MATH_PI / 2;
    if (pitch < -MATH_PI / 2)
        pitch = -MATH_PI / 2;
}

void camera_movePitch(double dp) {
    pitch += dp;
    if (pitch > MATH_PI / 2)
        pitch = MATH_PI / 2;
    if (pitch < -MATH_PI / 2)
        pitch = -MATH_PI / 2;
}

void camera_setYaw(double _yaw) {
    yaw = _yaw;
}

void camera_moveYaw(double dy) {
    yaw += dy;
}

void camera_move(float dx, float dz) {
    position += dz * glm::vec3(cos(yaw), 0, -sin(yaw));
    position += dx * glm::cross(glm::vec3(cos(yaw), 0, -sin(yaw)), glm::vec3(0, 1, 0));//  glm::vec3(-sin(yaw), 0, cos(yaw));
}

glm::mat4 camera_getMatrix() {
    glm::vec3 dir = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), -sin(yaw) * cos(pitch));
    
    return glm::lookAt(position, position + dir, glm::vec3(0, 1, 0));
}