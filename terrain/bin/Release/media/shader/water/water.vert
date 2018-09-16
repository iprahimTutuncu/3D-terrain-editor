#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texel;

out vec3 outPosition;
out vec3 outNormal;
out vec4 outColor;
out vec4 outClipSpace;
out vec2 outTexel;
out vec3 outDistToCamera;

uniform mat4 model;
uniform mat4 viewProj;
uniform float height;
uniform vec3 cameraPosition;


void main()
{
    outPosition = (model*vec4(position, 1.0f)).xyz;
    outNormal = mat3(transpose(inverse(model))) * normal;
    outColor = vec4(position, 1.0);
    outTexel = texel * 6.0;
    outDistToCamera = cameraPosition - outPosition;

    vec4 clipSpace = viewProj * model * vec4(position + vec3(0.0, 5.0, 0.0), 1.0f);
    outClipSpace = clipSpace;

    gl_Position = clipSpace;
}
