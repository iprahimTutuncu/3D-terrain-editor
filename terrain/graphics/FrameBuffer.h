#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <vector>

class FrameBuffer
{
    public:
        FrameBuffer();
        FrameBuffer(int w, int h);

        void init();
        bool isUsable();

        void setRenderBuffer(GLenum internalformat);
        GLuint genTextureColorBuffer();
        GLuint genTextureDepthShadowBuffer();

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
