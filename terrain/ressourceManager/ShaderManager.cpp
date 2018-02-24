#include "ShaderManager.h"

namespace RessourceManager{
    ShaderManager::ShaderManager()
    {

    }

    std::shared_ptr<Shader> ShaderManager::get(const std::string& vertPath, const std::string& fragPath)
    {
        const auto i = p_shaders.find(vertPath + fragPath);
        if(i != p_shaders.end())
            return i->second;
        else{
            std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
            p_shaders.insert({vertPath + fragPath, shader});
            return shader;
        }
    }

    void ShaderManager::removeUnused()
    {
        for(auto i = p_shaders.begin(); i != p_shaders.end();)
            if(i->second.unique())
                i = p_shaders.erase(i);
            else
                ++i;
    }

    std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::p_shaders;
}
