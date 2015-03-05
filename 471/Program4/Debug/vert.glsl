attribute vec4 aPosition;
attribute vec3 aNormal;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform vec3 uLightPos;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform int uShadeModel;
uniform int uShowNormal;

varying vec3 vNormal;
varying vec3 vColor;
varying vec4 vWorldSpace;

void main()
{
    if (uShowNormal == 1) {
        vec4 worldSpace = uViewMatrix * uModelMatrix * aPosition;
        gl_Position = uProjMatrix * worldSpace;
        
        vColor = 0.5 * normalize(aNormal) + 0.5;
    }
    else if (uShadeModel == 0) {
        // Goraud
        vec4 worldSpace = uModelMatrix * aPosition;
        gl_Position = uProjMatrix * uViewMatrix * worldSpace;
        
        vec3 normal = normalize((uModelMatrix * vec4(aNormal, 0)).xyz);
        vec3 lightVector = normalize(uLightPos - worldSpace.xyz);
        
        float Id = dot(normal, lightVector);
        float Is = pow(dot(normal, normalize(lightVector - worldSpace.xyz)), Ushine);
        
        vColor = UsColor * Is + UdColor * Id + UaColor;
    }
    else {
        // Phong
        vWorldSpace = uModelMatrix * aPosition;
        gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPosition;
        
        
        vNormal = (uModelMatrix * vec4(aNormal, 0)).xyz;
    }
}
