#version 120

// Interpolated values from the vertex shaders
varying vec3 fragmentColor;
varying vec3 barycentric;

void main()
{
    float dist = distance(barycentric, vec3(0.5));
    
    vec4 fadeColor = vec4(1, 1, 1, 1);
    
    gl_FragColor = mix(vec4(fragmentColor, 1), fadeColor, dist * 1.5);
}