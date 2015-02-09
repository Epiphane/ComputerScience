attribute vec4 aPos;
attribute vec3 aNor;
uniform mat4 P;
uniform mat4 MV;

varying vec4 vPos;
varying vec3 vNor;
varying float depth;

void main()
{
	gl_Position = P * MV * aPos;
    
    vPos = P * MV * vec4(aNor, 0);
    vNor = aNor;
    
    depth = gl_Position.z;
}
