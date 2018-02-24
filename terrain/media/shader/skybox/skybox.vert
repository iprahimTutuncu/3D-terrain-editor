#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    TexCoords = position;

    gl_Position = (proj * view * vec4(position, 1.0)).xyww;
}
