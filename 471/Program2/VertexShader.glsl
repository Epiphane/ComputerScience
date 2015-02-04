#version 120

// Same for all executions of shader
uniform vec2 windowScale;
uniform vec2 windowCenter;

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition;

uniform vec3 circle;
uniform float radius;
uniform float time;
uniform float speed;

void main() {
    
    float maxDist = length(vertexPosition) - radius;
    float movement = clamp(time * speed, 0, maxDist);
    //movement = mod(time * speed, maxDist * 2);
    //if(movement > maxDist) {
    //    movement = maxDist - (movement - maxDist);
    //}
    
    vec3 position = vec3(vertexPosition + movement * -normalize(vertexPosition));
    
    gl_Position = vec4(position.x * windowScale.x,
                       position.y * windowScale.y,
                       position.z, 1.0);
    
    gl_PointSize = 2;
}

