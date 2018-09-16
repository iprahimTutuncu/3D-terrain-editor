#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

struct LightProperties{
    bool isEnable;
    bool isDirection;
    bool isPoint;
    bool isCursor;

    vec3 position;
    vec3 direction;

    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform LightProperties light[1];

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

    vec3 lightDir = normalize(light[0].direction);

    vec3 normal = normalize(TexCoords);

    vec3 halfDir = normalize(lightDir + normal);

    float diff = max(0.f, dot(normal, lightDir));
    float spec = max(0.f, dot(normal, halfDir));

    if(spec <= 0.9999 && spec >= -0.9999){
            spec *= 0.1618033;
    }else{
        spec *= 0.50;
    }

    vec3 ambient  = light[0].ambient;
    vec3 diffuse  = light[0].diffuse  * diff;
    vec3 specular = light[0].specular * spec;

    color = ambient + diffuse + specular;
    color = mix(colorSea, color, factor);
    FragColor = vec4(color, 1.0);
}
