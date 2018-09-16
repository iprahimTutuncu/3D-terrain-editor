#include "Water.h"
#include "../ressourceManager/TextureManager.h"
Water::Water(int width,int height):
    reflectionFBO(width, height),
    refractionFBO(width, height),
    height(0)
{
    init(20.f);
}

void Water::init(float length)
{
    grid.init(2.f, 512.f);
    glm::mat4 toCenterMatrix(1.0);
    toCenterMatrix = glm::translate(toCenterMatrix, glm::vec3(-512.0/2, 0.0, -512.0/2));
    grid.setWorldTransform(grid.getWorldTransform() * toCenterMatrix);
    height = grid.getPosition(1,1).y;

    //load texture
    DUDV = RessourceManager::TextureManager::get(DUDVpath);
    normalmap = RessourceManager::TextureManager::get(normalmapPath);
    //GENERATION DE LA TEXTURE DE REFLECITON
    reflectionFBO.init();
    reflectionFBO.bind();

    reflectionTexID = reflectionFBO.genTextureColorBuffer();
    reflectionFBO.setRenderBuffer(GL_DEPTH24_STENCIL8);

    if(reflectionFBO.isUsable())
        std::cout << "reflecitonFBO, ready to use!" << std::endl;

    reflectionFBO.unBind();

    refractionFBO.init();
    refractionFBO.bind();

    std::cout << "gen refraction..." << std::endl;
    refractionFBO.setRenderBuffer(GL_DEPTH24_STENCIL8);
    refractionTexID = refractionFBO.genTextureColorBuffer();
    refractionDepthTexID = refractionFBO.genTextureDepthWaterBuffer();
    if(refractionFBO.isUsable())
        std::cout << "refractionFBO ready to use!" << std::endl;

    refractionFBO.unBind();

}


void Water::bindReflection()
{
    reflectionFBO.bind();
}

void Water::bindRefraction()
{
    refractionFBO.bind();
}

void Water::unbind()
{
    reflectionFBO.unBind();
    refractionFBO.unBind();
}

GLuint Water::getReflectionTexture()
{
    return reflectionFBO.getTextureColorBuffer(0);
}

GLuint Water::getRefractionTexture()
{
    return refractionFBO.getTextureColorBuffer(0);

}

GLuint Water::getRefractionDepthTexture()
{
    return refractionFBO.getTextureDepthBuffer(0);
}

Water::~Water()
{

}

float Water::getHeight()
{
    return height;
}

void Water::setHeight(float height)
{
    this->height = height;
}

void Water::build()
{

}

glm::mat4 Water::getWorldTransform()
{
   return grid.getWorldTransform();
}

void Water::draw(Shader& shader)
{
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, this->getReflectionTexture());

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, this->getRefractionTexture());

    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, *DUDV);

    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, *normalmap);

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, this->getRefractionDepthTexture());

    static float time = 0.01;
    time += 0.001;
    if(time > 1.0){
        time = 0.0f;
    }

    shader.use();
    shader.setInt("waterReflection", 3);
    shader.setInt("waterRefraction", 7);
    shader.setInt("waterRefractionDepth", 10);
    shader.setFloat("far_plane", 1000.f);
    shader.setInt("DUDV", 8);
    shader.setInt("normalmap", 9);
    shader.setFloat("elapsedTime", time);
    shader.setFloat("height", height);
    grid.draw(shader);
}

