#version 120

// Same for all executions of shader
uniform int w_width;
uniform int w_height;
uniform float radius;

// Interpolated values from the vertex shaders

void main()
{
    
    float dist = distance(gl_FragCoord.xy, vec2(w_width / 2, w_height / 2));
    float maxDist = distance(vec2(0, 0), vec2(w_width / 2, w_height / 2));

    float x = dist / maxDist;
    float mixer = 0.8 / (1 + pow(2.71828, 32*x - 4)) + (1 - x) / 2;
    
    gl_FragColor = mix(vec4(0, 0, 0, 1), vec4(0.8f, 1, 0, 1), mixer);
    
}