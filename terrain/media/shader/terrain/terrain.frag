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
uniform MaterialProperties material;

vec3 pointLight(LightProperties light, vec3 normal, vec3 position, vec3 eyeDir);
vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir);
vec3 offset_lookup(sampler2D map, vec3 loc, vec2 offset, vec2 texelSize);

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

void main()
{

    vec3 normal     = normalize(inVertex.normal);
    vec3 rgb_normal = normalize(texture(material.texture_normalMap, inVertex.texel).rgb * 2.0 - 1.0);
    vec3 eyeDir     = normalize(eyePosition - inVertex.position);
    vec3 result     = vec3(0.0);
    vec3 c1 = vec3(0.2,0.5,0.0);
    vec3 c2 = vec3(0.6,0.05,0.0);

    float lv1 = 0.1;
    float weight = 1.0;
    float lv2 = 0.9;

    float t = normal.y;

    float lv = pow(1-t, 2) * lv1 + 2*(1-t) * t * weight + 2*t*lv2;

    float height = outHeight;

    result = mix(c2, c1, lv);

    if(!gl_FrontFacing)
        normal = -normal;

    for(int i = 0; i < NUM_LIGHT; i++)
        if(light[i].isEnable)
            if(light[i].isPoint)
                result += pointLight(light[i], normal, inVertex.position, eyeDir);
            else if(light[i].isDirection)
                result += directionalLight(light[i], normal, eyeDir);



    FragColor = vec4(result, 1.0);
}


vec3 pointLight(LightProperties light, vec3 normal, vec3 position, vec3 eyeDir){
    vec3 lightDir = light.position - position;
    float dist = length(lightDir);
    lightDir = lightDir / dist;

    //diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    //specular
    vec3 halfwayDir = normalize(lightDir + eyeDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    //light with texture map
    vec3 ambient  = light.ambient  *        texture(material.texture_diffuse, inVertex.texel).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.texture_diffuse, inVertex.texel).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular, inVertex.texel).rgb;

    //compilation des lumieres
    vec3 scattered = ambient + diffuse;
    vec3 reflected = specular;

    //attenuation
    float attenuation = 1.0 / (light.attenuationConstant  +
                         light.attenuationLinear    * dist +
                         light.attenuationQuadratic * dist * dist);

    scattered *= attenuation;
    reflected *= attenuation;

    return material.emission + scattered + reflected;
}

vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir){
    vec3 lightDir   = normalize(light.direction);
    vec3 halfwayDir = normalize(lightDir + eyeDir);

    float diff = max(0.0, dot(normal, light.direction));
    float spec = max(0.0, dot(normal, halfwayDir));

    if(diff == 0.0)
        spec = 0.0;
    else
        spec = pow(spec, material.shininess);

    vec3 ambient  = light.ambient  *        texture(material.texture_diffuse, inVertex.texel).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.texture_diffuse, inVertex.texel).rgb;
    vec3 specular = light.specular * spec * texture(material.texture_specular, inVertex.texel).rgb;

    //compilation des lumieres

    float shadowVal = ShadowCalculation(inVertex.shadowCoord, lightDir);
    vec3 color = ambient + (1.0 - shadowVal) * (diffuse + specular);

    return material.emission + color;
}

vec3 offset_lookup(sampler2D map, vec3 loc, vec2 offset, vec2 texelSize){
    return texture(depth_texture, loc.xy + offset * texelSize);
}
