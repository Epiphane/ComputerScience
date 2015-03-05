#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {
    gl_Position = vec4(0, 0, 0, 1);
    EmitVertex();
    gl_Position = vec4(1, 0, 0, 1);
    EmitVertex();
    gl_Position = vec4(0, 1, 0, 1);
    EmitVertex();
    
//    for(int i = 0; i < 3; i++) { // You used triangles, so it's always 3
//        gl_Position = gl_in[i].gl_Position;
//        EmitVertex();
//    }
    EndPrimitive();
}