#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

const float lowerLimite = -2.0;
const float upperLimite = -2.0;

void main()
{
    vec4 color = texture(skybox, TexCoords);

    float factor = (TexCoords.y - lowerLimite) / (upperLimite - lowerLimite);
    factor = clamp(factor, 0.0, 1.0);

    FragColor = mix(vec4(0.0,0.0,1.0,1.0), color, factor);
}
