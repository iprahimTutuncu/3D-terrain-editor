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

in vec3 outTangent;

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



struct MaterialProperties{
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normalMap;
    vec3 emission;
    float shininess;
};

uniform sampler2D depth_texture;
uniform sampler2D DUDV;
uniform vec3 eyePosition;
uniform float elapsedTime;

in float outHeight;
in float outTexHeight;
#define NUM_LIGHT 4
uniform LightProperties light[NUM_LIGHT];

#define NUM_TEXTURE_TERRAIN 10
uniform MaterialProperties materials[NUM_TEXTURE_TERRAIN];
uniform int       num_texture_size;


vec3 pointLight(LightProperties light, vec3 normal, vec3 position, vec3 eyeDir,vec3 diffuse_color,vec3 specular_color);
vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir,vec3 diffuse_color,vec3 specular_color);
vec3 offset_lookup(sampler2D map, vec3 loc, vec2 offset, vec2 texelSize);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir);
float LinearizeDepth(float depth);

//variable globale
float lv = 0;

float far_plane = 1000.f;
float near_plane = 0.1f;

void main()
{

    //A UTILISER POUR LINTERPOLATION DES TEXTURES!
    vec3 normal     = normalize(inVertex.normal);


    vec3 eyeDir     = normalize(inVertex.tangentViewPosition - inVertex.position);
    vec3 result = vec3(0.0);
    vec3 c1 = vec3(0.2,0.5,1.0);
    vec3 c2 = vec3(0.6,0.05,0.0);

    //VARIABLES LIEES AUX TEXTURAGES DU TERRAIN
    float weight = 1.0;
    int lvMin = 0;
    int lvMax = 1;
    float t = normal.y;

    float lv1 = 0.0;
    float lv2 = 1.0;

    lv = pow(1-t, 2) * lv1 + 2*(1-t) * t * weight + 2*t*lv2;

    float height = outHeight;

    //INVERSER LES NORMALES S'ILS FONT PAS FACE
    if(!gl_FrontFacing)
        normal = -normal;

    //mix les 2 texture de gazon pour mix ensuite ici
    //use outTexHeight
    vec3 mixedFLoor_diff;
    vec3 mixedFLoor_spec;
    vec3 diffuse_color;
    vec3 specular_color;

    float d = length(inVertex.position - eyePosition);
    if(d > 200.f)
        d = 30;
    else if(d > 100.f)
        d = 40.f;
    else if(d > 90.f)
        d = 50.f;
    else if(d > 60.f)
        d = 90.f;
    else if(d > 30.f)
        d = 120.f;
    else if(d > 20.f)
        d = 200.f;
    else
        d = 256.f;

    //A UTILISER POUR LA COULEUR EN LIEN AVEC LA LUMIERE!
    // A MODIFIER: enfaite, modifier la nm selon le materiel courrant
    vec3 rgb_normal0 = normalize(texture(materials[0].texture_normalMap, inVertex.texel * d).rgb * 2.0 - 1.0);
    vec3 rgb_normal1 = normalize(texture(materials[1].texture_normalMap, inVertex.texel * d).rgb * 2.0 - 1.0);
    vec3 rgb_normal2 = normalize(texture(materials[2].texture_normalMap, inVertex.texel * d).rgb * 2.0 - 1.0);

    vec3 bitangent = normalize(cross(outTangent, normal));
    mat3 TBN = mat3(outTangent,bitangent,normal);

    rgb_normal0 = normalize(TBN * rgb_normal0);
    rgb_normal1 = normalize(TBN * rgb_normal1);
    rgb_normal2 = normalize(TBN * rgb_normal2);

    mixedFLoor_diff = mix(texture(materials[1].texture_diffuse , inVertex.texel * d).rgb,
                              texture(materials[2].texture_diffuse , inVertex.texel * d).rgb,
                              outTexHeight);

    mixedFLoor_spec = mix(texture(materials[1].texture_specular, inVertex.texel * d).rgb,
                              texture(materials[2].texture_specular, inVertex.texel * d).rgb,
                              outTexHeight);

    vec3 mixedFLoor_normalMap = normalize(mix(rgb_normal1, rgb_normal2, outTexHeight));

    diffuse_color = mix(texture(materials[0].texture_diffuse , inVertex.texel * d).rgb,
                        mixedFLoor_diff,
                        lv);

    specular_color =  mix(texture(materials[0].texture_specular, inVertex.texel * d).rgb,
                        mixedFLoor_spec,
                        lv);
    vec3 rgb_normal = rgb_normal0; //normalize(mix(rgb_normal0, mixedFLoor_normalMap, lv));

    //diffuse_color += vec3(outTexHeight,0,0);

    //AJOUT DES LUMIERES
    for(int i = 0; i < NUM_LIGHT; i++)
        if(light[i].isEnable)
            if(light[i].isPoint)
                result += pointLight(light[i], rgb_normal, inVertex.position, eyeDir, diffuse_color, specular_color);
            else if(light[i].isDirection)
                result += directionalLight(light[i], rgb_normal, eyeDir, diffuse_color, specular_color);

    float waterLevel = 5.f;
    far_plane = 75.f;
    float floorDist = LinearizeDepth(gl_FragCoord.z)  / far_plane;
    float waveStrength = 0.1;

    vec2 dudv = (texture(DUDV, inVertex.texel + vec2(elapsedTime, 0.0f)).rg*2.0-1.0) * waveStrength;

    vec3 underWaterColor = vec3(0.0,0.1,0.2);
    if(inVertex.position.y < waterLevel && eyePosition.y < waterLevel){
        result *= 1.0f - floorDist;
        result = mix(result, underWaterColor, floorDist) + (vec3(dudv.x) + vec3(0.f, dudv)) * 0.5;

    }

    FragColor = vec4(result, 1.0);
    //FragColor = vec4(rgb_normal, 1.0);
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

vec3 pointLight(LightProperties light, vec3 normal, vec3 position, vec3 eyeDir,vec3 diffuse_color,vec3 specular_color){

    if(light.isCursor){
        vec3 lightDir =  light.position - position;
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

        float min_length = 1.0 / (light.attenuationLinear + light.attenuationQuadratic);

        if(dist < min_length + 0.1 && dist > min_length)
            return vec3(0.0, 0.0, 1.0);
        else if(dist > min_length + 0.04)
            return vec3(0.0);

        return scattered + reflected;

    }
    else{
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
}

vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir,vec3 diffuse_color,vec3 specular_color){
    vec3 lightDir   = normalize(light.direction);
    vec3 halfwayDir = normalize(lightDir + eyeDir);

    float diff = max(0.0, dot(normal, light.direction));
    float spec = max(0.0, dot(normal, halfwayDir));

    if(diff == 0.0)
        spec = 0.0;
    else
        spec = pow(spec, 256);

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
