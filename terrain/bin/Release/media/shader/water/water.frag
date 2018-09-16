#version 330 core

out vec4 FragColor;
in vec4 outClipSpace;
in vec2 outTexel;
in vec3 outDistToCamera;
in vec3 outPosition;
uniform sampler2D waterReflection;
uniform sampler2D waterRefraction;
uniform sampler2D waterRefractionDepth;
uniform float far_plane;
uniform sampler2D normalmap;
uniform sampler2D DUDV;
uniform vec3 cameraFront;
uniform vec3 cameraPosition;

uniform float elapsedTime;

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

//uniform vec3 cameraPosition;
vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir,vec3 diffuse_color,vec3 specular_color, float waterDepth);

float near_plane = 0.1f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - depth * (far_plane - near_plane));
}

const float waveStrength = 0.05;

void main()
{
    vec2 ndc = (outClipSpace.xy/outClipSpace.w) / 2.0 + 0.5;
    vec2 refractTexel = ndc;
    vec2 reflectTexel = vec2(ndc.x, -ndc.y);
    float depth = 1.0f;

    depth = texture(waterRefractionDepth, refractTexel).r;
    float floorDist = LinearizeDepth(depth)  / far_plane;

    depth = gl_FragCoord.z;
    float waterSurfDist = LinearizeDepth(depth)  / far_plane;
    float waterDepth = floorDist - waterSurfDist;


    vec3 eyeDir     = normalize(cameraPosition - outPosition);
    vec3 distToCamera = normalize(outDistToCamera);

    float reflectionLv = pow(dot(distToCamera, vec3(0.0, 1.0, 0.0)), 2.0f);

    vec2 dudv = (texture(DUDV, outTexel + vec2(elapsedTime, 0.0f)).rg*2.0-1.0) * waveStrength * (1-reflectionLv) * clamp(waterDepth*5.0, 0.0, 1.0);

    vec3 normalRGB = texture(normalmap, dudv).xyz;
    normalRGB = vec3(normalRGB.r * 2.0 -1.0, normalRGB.b, normalRGB.g * 2.0 -1.0);
    normalRGB = normalize(normalRGB);
/*
    vec2 dudv = (texture(DUDV, outTexel + vec2(elapsedTime, 0.0f)).rg*2.0-1.0) * waveStrength * clamp(waterDepth*5.0, 0.0, 1.0);

    vec3 normalRGB = texture(normalmap, dudv).xyz;
    normalRGB = vec3(normalRGB.r * 2.0 -1.0, normalRGB.b * 5.0, normalRGB.g * 2.0 -1.0);
    normalRGB = normalize(normalRGB);

    float reflectionLv = pow(dot(eyeDir, normalRGB), 1.0f);
*/
    refractTexel += dudv;
    reflectTexel += dudv;

    refractTexel = clamp(refractTexel, 0.001, 0.999);
    reflectTexel.x = clamp(reflectTexel.x, 0.001, 0.999);
    reflectTexel.y = clamp(reflectTexel.y, -0.999, -0.001);

    vec3 color1 = texture(waterReflection, reflectTexel).xyz;
    vec3 color2 = texture(waterRefraction, refractTexel).xyz;
    vec3 result = mix(color1, color2, reflectionLv);
    result += directionalLight(light[0], normalRGB, eyeDir, result, vec3(1.0,1.0,1.0), waterDepth) * 0.5;
    //result *= vec3(0.9, 0.1, 0.1);

    vec3 underWaterColor = vec3(0.0,0.1,0.2);
    if(!gl_FrontFacing){
        floorDist = LinearizeDepth(depth)  / 50.f;

        result *= 1.0f - floorDist;
        result = mix(result, underWaterColor , floorDist);

        result *= underWaterColor;

        FragColor = vec4(result,1.0);
        FragColor.a = clamp(1.0 - waterDepth/4, 0.0, 1.0);
    }else{
        FragColor = vec4(result,1.0);
        FragColor.a = clamp(1.0 - waterDepth, 0.0, 1.0);
    }
}

vec3 directionalLight(LightProperties light, vec3 normal,vec3 eyeDir,vec3 diffuse_color,vec3 specular_color, float waterDepth){
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
    vec3 specular = light.specular * spec * clamp(waterDepth*200.0, 0.0, 1.0);
    //compilation des lumieres

    vec3 color = ambient + diffuse + specular;
    return color;
}
