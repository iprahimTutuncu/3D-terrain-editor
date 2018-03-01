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

    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mImage.getSize().x, mImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage.getPixelsPtr());

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void HeightMap::addCircle(float height, int x, int y, float attLinear,float attQuad){
    sf::Vector2u s = mImage.getSize();
    for(int i = 0; i < s.x; i++)
        for(int j = 0; j < s.y; j++){
            int uv = i + j * s.x;
            int texel = x + y * s.y;


            float dx = i - x;
            float dy = j - y;
            float d = sqrt(dx*dx + dy*dy);
            if(d < 50){
                std::cout << d << std::endl;
                sf::Color newColor = mImage.getPixel(i,j);

                float h = height * (1.0 / (1.0 + attLinear * d + attQuad * (d * d)));
                h+= newColor.r;
                if(h >= 255)
                {
                    newColor.r = 255;
                    newColor.g = 255;
                    newColor.b = 255;
                }else{
                    newColor.r = h;
                    newColor.g = h;
                    newColor.b = h;
                }

                mImage.setPixel(i, j, newColor);
                    //std::cout << "texel modifier" << i << ", " << j << std::endl;
            }
        }

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
