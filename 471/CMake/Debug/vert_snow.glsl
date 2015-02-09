attribute vec3 aPos;

uniform float time;
uniform float speed;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    float angle = aPos.x + time * speed * rand(vec2(speed * aPos.x));
    gl_Position = vec4(aPos.y * cos(angle), aPos.y * sin(angle), 0.1, 1);
    
    gl_PointSize = 20.0;
}
