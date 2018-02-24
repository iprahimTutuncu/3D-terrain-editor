#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "../ressourceManager/TextureManager.h"

#include <iostream>
#include <string>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;

#include "../core/SceneComponent.h"


class Model: public SceneComponent
{
    public:
        Model(const string &path);
        void Draw(Shader &shader);
        void setEmitColor(glm::vec3 color);

    private:
        vector<Mesh> meshes;

        string directory;

        void loadModel(const string &path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

};

#endif // MODEL_H
