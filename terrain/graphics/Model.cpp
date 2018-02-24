#include "Model.h"


Model::Model(const string &path):
    SceneComponent("Model")
{

    loadModel(path);
}

void Model::Draw(Shader &shader)
{
    for(Mesh mesh: meshes)
        mesh.draw(shader);
}

void Model::loadModel(const string &path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);


}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{

    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        Vertex vertex;

        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        /*
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.tangent = vector;

        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.bitangent = vector;
        */
        if(mesh->mTextureCoords[0]){
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vec;

        }else
            vertex.texCoord = glm::vec2(0.0f, 0.0f);


        vertices.push_back(vertex);
    }

    for ( GLuint i = 0; i < mesh->mNumFaces; i++ ){
        aiFace face = mesh->mFaces[i];
        for ( GLuint j = 0; j < face.mNumIndices; j++ )
            indices.push_back( face.mIndices[j] );
    }

    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        //diffuse map
        vector<Texture> diffuseMaps = this->loadMaterialTextures( material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert( textures.end( ), diffuseMaps.begin( ), diffuseMaps.end( ) );
        //specular map
        vector<Texture> specularMaps = this->loadMaterialTextures( material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert( textures.end( ), specularMaps.begin( ), specularMaps.end( ) );
        //reflection map
        vector<Texture> reflectionMaps = this->loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");

        textures.insert( textures.end(), reflectionMaps.begin(), reflectionMaps.end() );

        std::string typeName = "texture_reflection";
        auto it = find_if(textures.begin(), textures.end(), [&typeName](const Texture& t) {return t.type == typeName;});
        if (it == textures.end())
            textures.emplace_back(Texture{*RessourceManager::TextureManager::get("media/texture/default.png"), typeName, aiString("None")});

        typeName = "texture_diffuse";
        it = find_if(textures.begin(), textures.end(), [&typeName](const Texture& t) {return t.type == typeName;});
        if (it == textures.end())
            textures.emplace_back(Texture{*RessourceManager::TextureManager::get("media/texture/default.png"), typeName, aiString("None")});


        typeName = "texture_specular";
        it = find_if(textures.begin(), textures.end(), [&typeName](const Texture& t) {return t.type == typeName;});
        if (it == textures.end())
            textures.emplace_back(Texture{*RessourceManager::TextureManager::get("media/texture/default.png"), typeName, aiString("None")});

   }


    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{

    vector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){

        aiString str;
        mat->GetTexture( type, i, &str );

        Texture texture;
        texture.ID = *RessourceManager::TextureManager::get(directory + "/" + str.C_Str());
        texture.type = typeName;
        texture.path = str;
        textures.push_back(texture);

    }
    return textures;
}

void Model::setEmitColor(glm::vec3 color)
{
    for(Mesh &mesh: meshes)
        mesh.setEmissionColor(color);
}
