#include "HeightMap.h"

#include "../ressourceManager/TextureManager.h"

#include <iostream>
#include "glm/glm.hpp"
#include <math.h>

HeightMap::HeightMap(std::string path)
{
    mPath = path;
    mImage.loadFromFile(mPath);
    glGenTextures(1, &mTextureID);
    reload();

    mSize.x = mImage.getSize().x;
    mSize.y = mImage.getSize().y;
}

GLuint HeightMap::getTextureID()
{
    return mTextureID;
}

void HeightMap::reload()
{
    mImage.loadFromFile(mPath);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mImage.getSize().x, mImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage.getPixelsPtr());

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec3 HeightMap::getPixelColor(int i,int j)
{
    glm::vec3 color;
    if(j < 0 || i < 0)
        return glm::vec3(0,0,0);
    sf::Color pixelColor = mImage.getPixel(i,j);
    color.r = pixelColor.r;
    color.g = pixelColor.g;
    color.b = pixelColor.b;

    return color;
}

void HeightMap::addCircle(float height, int x, int y, float attLinear,float attQuad){
    std::cout << "shit boi is this in hmp?: " << 1.0 / (attLinear * attQuad) << std::endl;
    sf::Vector2u s = mImage.getSize();
    float top = 0;
    for(int i = 0; i < s.x; i++)
        for(int j = 0; j < s.y; j++){
            int uv = i + j * s.x;
            int texel = x + y * s.y;


            float dx = i - x;
            float dy = j - y;
            float d = sqrt(dx*dx + dy*dy);
            if(d < 1.0 / (attLinear + attQuad)){
                sf::Color newColor = mImage.getPixel(i,j);
                top < newColor.r? top = newColor.r:top = top;

                float h = height;// * (1.0 / (1.0 + attLinear * d + attQuad * (d * d)));
                h+= newColor.r;
                if(h >= 255)
                {
                    newColor.r = 255;
                    newColor.g = 255;
                    newColor.b = 255;
                }else if(h <= 0)
                {
                    newColor.r = 0;
                    newColor.g = 0;
                    newColor.b = 0;
                }else{
                    newColor.r = h;
                    newColor.g = h;
                    newColor.b = h;
                }
                mImage.setPixel(i, j, newColor);
                //std::cout << "texel modifier" << i << ", " << j << std::endl;
            }
        }
    std::cout << "new height: " << top << std::endl;

    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mImage.getSize().x, mImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage.getPixelsPtr());
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 HeightMap::getSize()
{
    return mSize;
}



HeightMap::~HeightMap()
{

}
