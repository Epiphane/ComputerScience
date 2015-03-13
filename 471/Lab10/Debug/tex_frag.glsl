uniform sampler2D uTexUnit;

varying vec2 vTexCoord;
varying vec3 vColor;

void main(void) {
  vec4 BaseColor = vec4(vColor.x, vColor.y, vColor.z, 1);
  vec4 texColor1 = texture2D(uTexUnit, vTexCoord);

  gl_FragColor = texColor1;
    
    if (texColor1.b + 0.1 > texColor1.g + texColor1.r) {
        gl_FragColor.r = texColor1.b;
    }
}

