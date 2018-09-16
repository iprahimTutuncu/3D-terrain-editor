#version 420 core

layout (vertices = 4) out;

void main(){
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    gl_TessLevelOuter[0] = 2.0;
    gl_TessLevelOuter[1] = 3.0;
    gl_TessLevelOuter[2] = 2.0;
    gl_TessLevelOuter[3] = 4.0;

    gl_TessLevelInner[0] = 4.0;
    gl_TessLevelInner[1] = 3.0;
}
