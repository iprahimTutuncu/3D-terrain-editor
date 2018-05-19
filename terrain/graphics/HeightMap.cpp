#include "HeightMap.h"

#include "../ressourceManager/TextureManager.h"

#include <iostream>
#include "glm/glm.hpp"
#include <math.h>
#include "SFML/System/Vector3.hpp"
#include <utility>

HeightMap::HeightMap(std::string path, std::string texPath)
{
    mPath = path;
    mTexPath = texPath;

    mImage.loadFromFile(mPath);
    mTexImage.loadFromFile(texPath);

    glGenTextures(1, &mTextureID);
    glGenTextures(1, &mTextureMapID);

    reload();

    mSize.x = 512;
    mSize.y = 512;

}



GLuint HeightMap::getTextureID()
{
    return mTextureID;
}

GLuint HeightMap::getTextureMapID()
{
    return mTextureMapID;
}

void HeightMap::reload()
{
    //height Map
    mImage.loadFromFile(mPath);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mImage.getSize().x, mImage.getSize().y, 0, GL_RGBA, GL_FLOAT, mImage.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    //texture Map
    mTexImage.loadFromFile(mTexPath);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, mTextureMapID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexImage.getSize().x, mTexImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTexImage.getPixelsPtr());

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

float HeightMap::getRedColor(int i,int j)
{
    if(j < 0 || i < 0 || j > this->getSize().y || i > this->getSize().x)
        return 0;

    int uv = i + j * mSize.x;

    return *(data + uv);
}

void HeightMap::addCircle(float height, int x, int y, float attLinear,float attQuad, float r){
    sf::Vector2u s = sf::Vector2u(512, 512);

    for(int i = 0; i < s.x; i++)
        for(int j = 0; j < s.y; j++){
            int uv = i + j * s.x;

            float dx = i - x;
            float dy = j - y;
            float d = sqrt(dx*dx + dy*dy);
            if(d < 1.0 / (attLinear + attQuad)){
                float elevation = *(data + uv);

                float h = height * (1.0 / (r + attLinear * d + attQuad * d * d));
                h += elevation;

                if(h >= 1)
                    elevation = 1.f;
                else if(h <= 0)
                    elevation = 0.f;
                else{
                    elevation = h;
                }
                *(data + uv) = elevation;
            }
        }

    glBindTexture(GL_TEXTURE_2D, mTextureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mImage.getSize().x, mImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void HeightMap::addAveragingCircle(int x, int y, float attLinear, float attQuad)
{
    sf::Vector2u s;
    s.x = mSize.x;
    s.y = mSize.y;

    glm::f64 average{0};
    int counter = 0;

    for(int i = 0; i < s.x; i++)
        for(int j = 0; j < s.y; j++){
            int uv = i + j * s.x;

            float dx = i - x;
            float dy = j - y;
            float d = sqrt(dx*dx + dy*dy);
            if(d < 1.0 / (attLinear + attQuad)){
                float elevation = *(data + uv);
                average+= elevation;
                counter++;
            }
        }

    if(counter >= 1)
        average /= counter;
    else
        average = 0;

    for(int i = 0; i < s.x; i++)
        for(int j = 0; j < s.y; j++){
            int uv = i + j * s.x;

            float dx = i - x;
            float dy = j - y;
            float d = sqrt(dx*dx + dy*dy);
            if(d < 1.0 / (attLinear + attQuad)){
                float elevation = *(data + uv);
                if(elevation > average)
                    elevation-= 0.003;
                else if(elevation < average)
                    elevation+= 0.003;

                if(elevation > average - 0.003 && elevation < average + 0.003)
                    elevation = average;

                *(data + uv) = elevation;
            }
        }

    glBindTexture(GL_TEXTURE_2D, mTextureID);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mImage.getSize().x, mImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);


}

void HeightMap::addSmoothCircle(int x, int y, float attLinear, float attQuad)
{
    sf::Vector2u s;
    s.x = mSize.x;
    s.y = mSize.y;

    for(int i = 0; i < s.x; i++)
        for(int j = 0; j < s.y; j++){
            //int texel = x + y * s.y;
            int uv = i + j * s.x;

            float dx = i - x;
            float dy = j - y;
            float d = sqrt(dx*dx + dy*dy);

            if(d < 1.0 / (attLinear + attQuad)){
                glm::f64 average = 0;
                float elevation;
                //averaging
                int c = 0;
                for(int k = -1; k <= 1; k++)
                    for(int l = -1; l <= 1; l++){
                        if(i + k < 0 || j + l < 0)
                            continue;
                        if(i + k >= mSize.x || j + l >= mSize.y)
                            continue;
                        c++; // haha, je suis drole xD
                        int nearUV = (i+k) + (j+l) * s.x;
                        elevation = *(data + nearUV);
                        average += elevation;
                }

                average /= c;

                elevation = *(data + uv);
                average += elevation;
                average /= 2;

                elevation = average;

                if(elevation >= 1.f)
                    elevation = 1.f;
                else if(elevation <= 0.f)
                    elevation = 0.f;

                *(data + uv) = elevation;
            }
        }

    std::cout << x << ", " << y << std::endl;

    glBindTexture(GL_TEXTURE_2D, mTextureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mImage.getSize().x, mImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage.getPixelsPtr());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void HeightMap::addCircleOnTextureMap(float height, int x, int y, float attLinear, float attQuad)
{
    std::cout << "donc: " << x << ", " << y << std::endl;
    sf::Vector2u s = mTexImage.getSize();
    float top = 0;
    for(int i = 0; i < s.x; i++)
        for(int j = 0; j < s.y; j++){
            int uv = i + j * s.x;
            int texel = x + y * s.y;

            float dx = i - x;
            float dy = j - y;
            float d = sqrt(dx*dx + dy*dy);
            if(d < 1.0 / (attLinear + attQuad)){
                sf::Color newColor = mTexImage.getPixel(i,j);
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
                mTexImage.setPixel(i, j, newColor);
                //std::cout << "texel modifier" << i << ", " << j << std::endl;
            }
        }

    glBindTexture(GL_TEXTURE_2D, mTextureMapID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTexImage.getPixelsPtr());
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA32F, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}


glm::vec2 HeightMap::getSize()
{
    return mSize;
}



HeightMap::~HeightMap()
{

}
