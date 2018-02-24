#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <unordered_map>
#include <memory>
#include <GL/glew.h>

namespace RessourceManager{
    class TextureManager
    {
        public:
            TextureManager();
            static std::shared_ptr<GLuint> get(const std::string& name);
            static void removeUnused();
        private:
            static std::unordered_map<std::string, std::shared_ptr<GLuint>> p_textureIDs;
    };
}

#endif // TEXTUREMANAGER_H
