#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texel;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

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

void main()
{
    /*
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
*/

    outVertex.position = (model*vec4(position, 1.0f)).xyz;
    outVertex.normal   = mat3(transpose(inverse(model))) * normal;
    outVertex.texel    = texel;
    outVertex.shadowCoord = lightSpaceCoordinate * vec4(outVertex.position, 1.0);

    gl_Position = viewProj * model * vec4(position, 1.0f);
}
