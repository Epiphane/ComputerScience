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
    
    if(dist < radius * min(w_width, w_height) / 2)
        gl_FragColor = vec4(1, 0.95f, 0, 1);
    else
        gl_FragColor = vec4(0.4f, 0.9f, 0.8f, 1 - (dist - radius * min(w_width, w_height) / 2) / maxDist);

}