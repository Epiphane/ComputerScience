varying vec3 vNor;
varying vec3 vCol;

uniform int sphere;

void main()
{
    if(sphere >= 3) {
        vec3 light = vec3(1, 2, 3);
    
        gl_FragColor = vec4(vCol * dot(normalize(light), normalize(vNor)), 1);
    }
    else {
        gl_FragColor = vec4(vCol, 1);
    }
}
