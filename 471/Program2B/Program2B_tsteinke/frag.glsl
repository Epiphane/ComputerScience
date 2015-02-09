varying vec3 vNor;
varying vec4 vPos;
varying float depth;

uniform mat4 P;
uniform mat4 MV;

uniform int material;

void main()
{
    if(material == 0) {
        float direction = dot(vec3(0, 0, 1), normalize(vPos.xyz));
        if(direction < 0.1)
            discard;
    
        vec3 vCol = 0.5 * vec3(vPos + 1.0);
        vCol = vec3(3.0 - depth);
    
        if(direction < 0.2)
            gl_FragColor = vec4(0.3, 0.3, 0.3, 0.5);
        else
            gl_FragColor = vec4(vCol - 0.2, 1.0);
    }
    else if(material == 1) {
        gl_FragColor = vec4(0.5 * vec3(vNor + 1.0), 1.0);
    }
    else {
        gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0);
    }
}
