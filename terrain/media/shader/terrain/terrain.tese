#version 420 core

layout (quads, equal_spacing, ccw) in;

out vec4 color;

void main(){
    float u = gl_TessCoord.x;
    float omu = 1 - u;
    float v = gl_TessCoord.y;
    float omv = 1 - v;

    color = vec4(gl_TessCoord, 1.0);

    gl_Position =
        omu * omv * gl_in[0].gl_Position +
        u * omv * gl_in[1].gl_Position +
        u * v * gl_in[2].gl_Position +
        omu *v * gl_in[3].gl_Position;
}
