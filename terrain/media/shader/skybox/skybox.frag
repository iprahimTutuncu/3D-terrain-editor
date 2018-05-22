#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

const float lowerLimite = 0.0;
const float upperLimite = 0.7;


void main()
{
    vec3 color = texture(skybox, TexCoords).rgb;
    vec3 colorSun = vec3(0.3,0.4,0.9);
    vec3 colorMoon = vec3(0.2,0.0,0.2);

    float factor = (TexCoords.y - lowerLimite) / (upperLimite - lowerLimite);
    factor = clamp(factor, 0.0, 1.0);

    vec3 colorSea = vec3(0.0, 0.1, 0.2);
    FragColor = vec4(mix(colorSun, colorMoon, factor), 1.0);
    FragColor = vec4(mix(colorSea, color, factor), 1.0);
}
