#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthScreenTexture;
uniform float speed;

uniform bool gray_scale;

const float offset = 1.f / 300.f;
float near_plane = 0.1;
float far_plane = 1000.f;


float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{

    float depth = 1.0f;
    depth = texture(depthScreenTexture, TexCoords).r;
    depth = LinearizeDepth(depth) / far_plane;

/*
    float average = 0.0f;
    if(gray_scale){
        average = (col.r + col.g + col.b) / 3;
        col = vec3(average);
    }
*/
    vec3 col = texture(screenTexture, TexCoords).rgb;

    FragColor = vec4(vec3(col), 1.0);
}
