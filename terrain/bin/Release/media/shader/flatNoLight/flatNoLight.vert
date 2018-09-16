#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texel;

out vec3 outPosition;
out vec2 outTexel;
out vec3 outNormal;
out vec4 outColor;

uniform mat4 model;
uniform mat4 viewProj;
uniform vec3 color;



void main()
{
    outPosition = (model*vec4(position, 1.0f)).xyz;
    outNormal = mat3(transpose(inverse(model))) * normal;
    outTexel = texel;
    outColor = vec4(position, 1.0);

    gl_Position = viewProj * model * vec4(position, 1.0f);
}

