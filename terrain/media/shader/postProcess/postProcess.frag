#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool gray_scale;



void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    float average = 0.0f;
    if(gray_scale){
        average = (col.r + col.g + col.b) / 3;
        col = vec3(average);
    }

    FragColor = vec4(col, 1.0);
}
