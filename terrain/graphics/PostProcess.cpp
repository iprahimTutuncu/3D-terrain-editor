#include "PostProcess.h"

PostProcess::PostProcess(float width, float height)
:fbo(width, height)
{
    init();
}

PostProcess::~PostProcess()
{
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
}

bool PostProcess::init()
{
    fbo.init();
    fbo.genTextureColorBuffer();
    fbo.setRenderBuffer(GL_DEPTH24_STENCIL8);
    if(!fbo.isUsable())
        return false;

    float quadVertices[] = { //un ecran normalizer
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    shaderUniform[0] = "gray_scale";

    p_shader = RessourceManager::ShaderManager::get("media/shader/postProcess/postProcess.vert", "media/shader/postProcess/postProcess.frag");
    p_shader->use();
    p_shader->setInt("screenTexture", 0);

    for(std::string su: shaderUniform)
        p_shader->setBool(su, false);

    return true;
}

void PostProcess::beginDraw()
{
    fbo.bind();
}

void PostProcess::endDraw()
{
    fbo.unBind();
}

void PostProcess::drawScreen()
{
    glEnable(GL_FRAMEBUFFER_SRGB);
    glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glBindVertexArray(quadVAO);
    p_shader->use();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, fbo.getTextureColorBuffer(0));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
}

void PostProcess::setPostProcessMode(PostProcessMode postProcessMode)
{
    int val = static_cast<int>(postProcessMode);
    for(int i = 0; i < shaderUniform.size(); i++)
        if(i == val)
            p_shader->setBool(shaderUniform[i], true);
        else
            p_shader->setBool(shaderUniform[i], false);

}
