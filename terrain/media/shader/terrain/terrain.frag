#version 420 core

out vec4 FragColor;

in out_Vertex{
    vec3 position;
    vec3 normal;
    vec2 texel;
    vec4 shadowCoord;
    vec3 tangentLightPosition;
    vec3 tangentViewPosition;
    vec3 tangentFragPosition;
} inVertex;


struct LightProperties{
    bool isEnable;
    bool isDirection;
    bool isPoint;

    vec3 position;
    vec3 direction;

    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



struct MaterialProperties{
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normalMap;
    vec3 emission;
    float shininess;
};

uniform sampler2D depth_texture;
uniform vec3 eyePosition;

in float outHeight;
#define NUM_LIGHT 4
uniform LightProperties light[NUM_LIGHT];

#define NUM_TEXTURE_TERRAIN 10
uniform MaterialProperties materials[NUM_TEXTURE_TERRAIN];
uniform float     affected_normal_levels[NUM_TEXTURE_TERRAIN];
uniform int       num_texture_size;

vec3 pointLight(LightProperties light, vec3 normal, vec3 position, vec3 eyeDir,vec3 diffuse_color,vec3 specular_color);
vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir,vec3 diffuse_color,vec3 specular_color);
vec3 offset_lookup(sampler2D map, vec3 loc, vec2 offset, vec2 texelSize);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir);

//variable globale
float lv = 0;

void main()
{

    vec3 normal     = normalize(inVertex.normal);
    vec3 rgb_normal = normalize(texture(materials[0].texture_normalMap, inVertex.texel).rgb * 2.0 - 1.0);
    vec3 eyeDir     = normalize(eyePosition - inVertex.position);
    vec3 result = vec3(0.0);
    vec3 c1 = vec3(0.2,0.5,1.0);
    vec3 c2 = vec3(0.6,0.05,0.0);

    //VARIABLES LIEES AUX TEXTURAGES DU TERRAIN
    float weight = 1.0;
    int lvMin = 0;
    int lvMax = 0;
    float t = normal.y;

    //INTERPOLATION DES TEXTURES AVEC LES NORMALES
    for(int i = 0; affected_normal_levels[i] < t && i < num_texture_size; i++)
        lvMin = i;

    for(int i = num_texture_size; affected_normal_levels[i] > t && i > 0; i--)
        lvMax = i;

    float lv1 = 0.2; //affected_normal_levels[lvMin];
    float lv2 = 0.8; //affected_normal_levels[lvMax];

    lv = pow(1-t, 2) * lv1 + 2*(1-t) * t * weight + 2*t*lv2;

    float height = outHeight;

    //INVERSER LES NORMALES S'ILS FONT PAS FACE
    if(!gl_FrontFacing)
        normal = -normal;


    vec3 diffuse_color  = mix(texture(materials[0].texture_diffuse , inVertex.texel).rgb,
                              texture(materials[1].texture_diffuse , inVertex.texel).rgb,
                              lv);

    vec3 specular_color = mix(texture(materials[0].texture_specular, inVertex.texel).rgb,
                              texture(materials[1].texture_specular, inVertex.texel).rgb,
                              lv);

    //AJOUT DES LUMIERES
    for(int i = 0; i < NUM_LIGHT; i++)
        if(light[i].isEnable)
            if(light[i].isPoint)
                result += pointLight(light[i], normal, inVertex.position, eyeDir, diffuse_color, specular_color);
            else if(light[i].isDirection)
                result += directionalLight(light[i], normal, eyeDir, diffuse_color, specular_color);



    FragColor = vec4(result, 1.0);
}


vec3 pointLight(LightProperties light, vec3 normal, vec3 position, vec3 eyeDir,vec3 diffuse_color,vec3 specular_color){
    vec3 lightDir = light.position - position;
    float dist = length(lightDir);
    lightDir = lightDir / dist;

    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //specular
    vec3 halfwayDir = normalize(lightDir + eyeDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);

    //light with texture map
    vec3 ambient  = light.ambient  *        diffuse_color;
    vec3 diffuse  = light.diffuse  * diff * diffuse_color;
    vec3 specular = light.specular * spec * specular_color;

    //compilation des lumieres
    vec3 scattered = ambient + diffuse;
    vec3 reflected = specular;

    //attenuation
    float attenuation = 1.0 / (light.attenuationConstant  +
                         light.attenuationLinear    * dist +
                         light.attenuationQuadratic * dist * dist);

    scattered *= attenuation;
    reflected *= attenuation;

    return scattered + reflected;
}

vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir,vec3 diffuse_color,vec3 specular_color){
    vec3 lightDir   = normalize(light.direction);
    vec3 halfwayDir = normalize(lightDir + eyeDir);

    float diff = max(0.0, dot(normal, light.direction));
    float spec = max(0.0, dot(normal, halfwayDir));

    if(diff == 0.0)
        spec = 0.0;
    else
        spec = pow(spec, 32);

    vec3 ambient  = light.ambient  *        diffuse_color;
    vec3 diffuse  = light.diffuse  * diff * diffuse_color;
    vec3 specular = light.specular * spec * specular_color;
    //compilation des lumieres

    float shadowVal = ShadowCalculation(inVertex.shadowCoord, lightDir);
    vec3 color = ambient + (1.0 - shadowVal) * (diffuse + specular);

    return color;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords/2 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0;

    vec2 texelSize = 1.0 / textureSize(depth_texture, 0);

    float closestDepth = texture(depth_texture, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // retourne vraie si le fragment est cache par l'ombre
    float diff = dot(inVertex.normal, lightDir);
    float bias = max(0.0618033 * (1.0 - diff), 0.005);

    //anti-alias
    float shadow = 0.0;
    for(float x = -1.5; x <= 1.5; ++x)
        for(float y = -1.5; y <= 1.5; ++y)
        {
            float pcfDepth = offset_lookup(depth_texture, projCoords, vec2(x,y), texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }

    shadow /= 16.0;
    return shadow;
}

vec3 offset_lookup(sampler2D map, vec3 loc, vec2 offset, vec2 texelSize){
    return texture(depth_texture, loc.xy + offset * texelSize);
}
