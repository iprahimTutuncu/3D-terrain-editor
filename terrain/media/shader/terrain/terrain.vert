#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texel;

out out_Vertex{
    vec3 position;
    vec3 normal;
    vec2 texel;
    vec4 shadowCoord;
    vec3 tangentLightPosition;
    vec3 tangentViewPosition;
    vec3 tangentFragPosition;
} outVertex;

uniform mat4 model;
uniform mat4 modelView;
uniform mat4 viewProj;
uniform mat4 lightSpaceCoordinate;

uniform sampler2D heightmap;
uniform sampler2D texturemap;

uniform float terrainWidth;
uniform float terrainHeight;

out float outHeight;
out float outTexHeight;

uniform vec3 lightPosition;
uniform vec3 eyePosition;

uniform vec4 planeClip;

void main()
{
    vec3 HMpos = position;
    vec2 uv = vec2(position.x/terrainWidth, position.z/terrainWidth) + 0.5;
    float height = texture(heightmap, uv).r;
    if(height < 0.5f)
        ;//height = 1.0f - height;
    HMpos.y += height * 69.0f;



    vec3 textureColor = texture(texturemap, uv).rgb;
    outTexHeight = textureColor.r;
    // # P.xy store the position for which we want to calculate the normals
    // # height() here is a function that return the height at a point in the terrain

    //recherche des hauteurs dans les cotes de la normal avec un offset de la taille du terrrain.
    vec3 offset = vec3(1.0/terrainWidth, 1.0/terrainWidth, 0.0);
    float hL = texture(heightmap, uv - offset.xz).r;
    float hR = texture(heightmap, uv + offset.xz).r;
    float hD = texture(heightmap, uv - offset.zy).r;
    float hU = texture(heightmap, uv + offset.zy).r;

    //calcule qui fonctionne magiquement trouve sur stackoverflow
    //ca mevite un cross product
    vec3 nTerrain;
    nTerrain.x = hL - hR;
    nTerrain.y = 1.0/terrainWidth;
    nTerrain.z = hD - hU;
    nTerrain = normalize(nTerrain);

    // positions
    vec3 pos1 = vec3(-1.0,  1.0, 0.0);
    vec3 pos2 = vec3(-1.0, -1.0, 0.0);
    vec3 pos3 = vec3( 1.0, -1.0, 0.0);
    vec3 pos4 = vec3( 1.0,  1.0, 0.0);
    // texture coordinates
    vec2 uv1 = vec2(0.0, 1.0);
    vec2 uv2 = vec2(0.0, 0.0);
    vec2 uv3 = vec2(1.0, 0.0);
    vec2 uv4 = vec2(1.0, 1.0);

    vec3 edge1 = pos2 - pos1;
    vec3 edge2 = pos3 - pos1;
    vec2 deltaUV1 = uv2 - uv1;
    vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    vec3 tangent = vec3(1.0);

    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent = normalize(tangent);

    vec3 T = normalize(vec3(mat3(transpose(inverse(model))) * tangent));
    vec3 N = normalize(vec3(mat3(transpose(inverse(model))) * nTerrain));
    vec3 B = cross(N, T);

    mat3 TBN = mat3(T,B,N);

    vec4 worldPosition = model*vec4(HMpos, 1.0f);
    outVertex.position = worldPosition.xyz;
    outVertex.normal   = mat3(transpose(inverse(model))) * nTerrain;

    gl_ClipDistance[0] = dot(worldPosition, planeClip);


    float d = length(outVertex.position);

    outVertex.texel = texel;
    outVertex.shadowCoord = lightSpaceCoordinate * vec4(outVertex.position, 1.0);
    outHeight = height;

    outVertex.tangentLightPosition = TBN * lightPosition;
    outVertex.tangentViewPosition = TBN * eyePosition;
    outVertex.tangentFragPosition = TBN * outVertex.position;

    gl_Position = viewProj * model * vec4(HMpos , 1.0f);
}
