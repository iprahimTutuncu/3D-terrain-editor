#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <memory>
#include <unordered_map>
#include <string>

#include "../graphics/Shader.h"

namespace RessourceManager{
    class ShaderManager
    {
        public:
            ShaderManager();
            static std::shared_ptr<Shader> get(const std::string& vertPath, const std::string& fragPath);
            static void removeUnused();
        private:
            static std::unordered_map<std::string, std::shared_ptr<Shader>> p_shaders;
    };
}
#endif // SHADERMANAGER_H
