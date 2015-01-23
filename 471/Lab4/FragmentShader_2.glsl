#version 120

// Window sizes
uniform int w_width;
uniform int w_height;
uniform vec2 windowCenter;

// Interpolated values from the vertex shaders
varying vec3 fragmentColor;
varying vec3 barycentric;

void main()
{
    float dist = distance(gl_FragCoord.xy, windowCenter);
    if(dist < 20)
        discard;

    float maxDist = min(w_width, w_height) / 2.15;
    vec4 fadeColor = vec4(1, 1, 1, 1);
    
    gl_FragColor = mix(vec4(fragmentColor, 1), fadeColor, dist / maxDist);
}