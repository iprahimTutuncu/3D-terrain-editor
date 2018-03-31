#include "TextureManager.h"
#include "SFML/Graphics.hpp"
#include <iostream>

namespace RessourceManager{
    TextureManager::TextureManager()
    {
        //potentiellement faire une texture par default
        //utiliser cette texture par default  pour quand il existe point de texture
    }

    std::shared_ptr<GLuint> TextureManager::get(const std::string& name)
    {
        const auto i = p_textureIDs.find(name);
        if(i != p_textureIDs.end())
            return i->second;
        else{
            std::shared_ptr<GLuint> p_textureID = std::make_shared<GLuint>();

            glGenTextures(1, p_textureID.get());


            sf::Image image;
            image.loadFromFile(name);

            glBindTexture(GL_TEXTURE_2D, *p_textureID.get());
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture( GL_TEXTURE_2D, 0 );
            p_textureIDs.insert({name, p_textureID});

            return p_textureID;
        }
    }

    void TextureManager::removeUnused()
    {
        for(auto i = p_textureIDs.begin(); i != p_textureIDs.end();)
            if(i->second.unique())
                i = p_textureIDs.erase(i);
            else
                ++i;
    }


    std::unordered_map<std::string, std::shared_ptr<GLuint>> TextureManager::p_textureIDs;
}
