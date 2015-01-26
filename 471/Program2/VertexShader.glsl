#version 120
// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition;

uniform vec3 circle;
uniform float radius;
uniform float time;
uniform float speed;

void main() {
    
//    float distFromCircle = distance(vertexPosition, circle) - radius;
    float movement = clamp(time * speed, 0, length(vertexPosition) - radius);
//    movement = 0;
    
    gl_Position = vec4(vertexPosition, 1.0);
    gl_Position = vec4(vertexPosition + movement * -normalize(vertexPosition), 1.0);
    gl_PointSize = 20;
}

