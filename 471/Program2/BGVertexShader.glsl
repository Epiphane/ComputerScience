#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition;

void main() {
    
    gl_Position = vec4(vertexPosition, 1);
    
}

