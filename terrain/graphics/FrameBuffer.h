#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <vector>

class FrameBuffer
{
    public:
        FrameBuffer();
        FrameBuffer(const int &w,const int &h);

        void init();
        bool isUsable();

        void setRenderBuffer(GLenum internalformat);
        GLuint genTextureColorBuffer();
        GLuint genTextureColorBuffer2();
        GLuint genTextureDepthShadowBuffer();
        GLuint genTextureDepthWaterBuffer();

        void bind();
        void unBind();

        GLuint getID() const;
        GLuint getTextureColorBuffer(unsigned int index) const;
        GLuint getTextureDepthBuffer(unsigned int index) const;

        int getWidth() const;
        int getHeight() const;

        ~FrameBuffer();

    private:
        GLuint ID;

        int width;
        int height;

        std::vector<GLuint> colorBufferIDs;
        std::vector<GLuint> depthBufferIDs;
        GLuint renderBufferID;

};

#endif // FRAMEBUFFER_H
