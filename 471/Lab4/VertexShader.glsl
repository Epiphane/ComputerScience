#version 120

// Same for all executions of shader
uniform vec2 windowScale;

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition;
attribute vec3 vertexColor;
attribute vec3 vertexBC;

varying vec3 fragmentColor;
varying vec3 barycentric;

void main(){

    gl_Position = vec4(vertexPosition.x * windowScale.x,
                       vertexPosition.y * windowScale.y,
                       vertexPosition.z, 1.0);
    
    barycentric = vertexBC;
    // Set the fragmemt color for the vertex shader
    fragmentColor = vertexColor;
}

