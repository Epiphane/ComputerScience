attribute vec4 aPos;
attribute vec3 aNor;
uniform mat4 P;
uniform mat4 MV;
varying vec3 vNor;
varying vec3 vCol;

uniform int sphere;

void main()
{
	gl_Position = P * MV * aPos;
    
    if(sphere >= 3) { // Phong
        if(sphere == 3)
            vNor = aNor;
        else
            vNor = (MV * vec4(aNor, 0)).xyz;
        
        vCol = vec3(0, 0.9, 0.9);
    }
    else { // Gouraud
        vec3 light = vec3(1, 2, 3);
        float i;
        if(sphere == 1)
            i = dot(normalize(aNor), normalize(light));
        else
            i = dot(normalize((MV * vec4(aNor, 0)).xyz), normalize(light));
        
        vCol = vec3(0, 0.9, 0.9) * i;
    }
}
