#version 120

// Window sizes
uniform int w_width;
uniform int w_height;

// Interpolated values from the vertex shaders
varying vec3 fragmentColor;
varying vec3 barycentric;

void main()
{
    if(gl_FragCoord.y > w_height / 2) {
        gl_FragColor = vec4(0, 0, 0.4, 1);
    }
    else {
        gl_FragColor = vec4(fragmentColor, 1);
    }
}