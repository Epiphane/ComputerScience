#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition;
attribute vec3 vertexColor;

varying vec3 fragmentColor;

void main() {
    
    gl_Position = vec4(vertexPosition, 1.0);
    
    fragmentColor = vertexColor;
}

