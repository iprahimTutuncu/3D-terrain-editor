#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "FrameBuffer.h"
#include "ShaderManager.h"

/**
*   This enum is alligned pricesly with
*   shaderUniform array. Otherwise, it
*   won't work properly.
**/

enum class PostProcessMode {GRAY_SCALE};


class PostProcess
{
    public:
        PostProcess(float width, float height);
        ~PostProcess();

        bool init();

        void beginDraw();
        void endDraw();
        void drawScreen();

        void setPostProcessMode(PostProcessMode postProcessMode);

    private:
        FrameBuffer fbo;
        GLuint quadVAO;
        GLuint quadVBO;

        std::array<std::string, 1> shaderUniform;
        std::shared_ptr<Shader> p_shader;
};

#endif // POSTPROCESS_H
