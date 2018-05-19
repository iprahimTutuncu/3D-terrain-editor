#include "FrameBuffer.h"

#include <iostream>

FrameBuffer::FrameBuffer():
    ID(0), width(0), height(0), colorBufferIDs(0), renderBufferID(0)
{

}

FrameBuffer::FrameBuffer(const int &w,const int &h):
    ID(0),
    width(w),
    height(h),
    colorBufferIDs(0),
    renderBufferID(0)
{

}

void FrameBuffer::init()
{
    if(glIsFramebuffer(ID) == GL_TRUE){
        glDeleteFramebuffers(1, &ID);
        colorBufferIDs.clear();
    }

    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

}

bool FrameBuffer::isUsable(){
    auto fbostatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fbostatus != GL_FRAMEBUFFER_COMPLETE){
        glDeleteFramebuffers(1, &ID);
        glDeleteRenderbuffers(1, &renderBufferID);
        colorBufferIDs.clear();
        std::cout << "ERROR::FRAMEBUFFER: Framebuffer incomplet: " << fbostatus << std::endl;
        return false;
    }

    return true;
}

void FrameBuffer::setRenderBuffer(GLenum internalformat)
{
    if(glIsRenderbuffer(renderBufferID) == GL_TRUE){
        glDeleteRenderbuffers(1, &renderBufferID);
        colorBufferIDs.clear();
    }

    glGenRenderbuffers(1, &renderBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
}

GLuint FrameBuffer::genTextureColorBuffer()
{
    GLuint textureColorID;
    glGenTextures(1, &textureColorID);
    glBindTexture(GL_TEXTURE_2D, textureColorID);
    std::cout << "width :" << width << ", height :" << height << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorBufferIDs.size(), GL_TEXTURE_2D, textureColorID, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    colorBufferIDs.emplace_back(textureColorID);
    std::cout << colorBufferIDs.back() << std::endl;

    return textureColorID;


}

GLuint FrameBuffer::genTextureDepthShadowBuffer()
{
    bind();

    GLuint textureDepthBufferID;

    glGenTextures(1, &textureDepthBufferID);
    glBindTexture(GL_TEXTURE_2D, textureDepthBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    //filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //border
    float couleurExtremite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, couleurExtremite);

    //wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthBufferID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    depthBufferIDs.emplace_back(textureDepthBufferID);
    std::cout << depthBufferIDs.back() << std::endl;

    unBind();
    return textureDepthBufferID;

}

GLuint FrameBuffer::genTextureDepthWaterBuffer()
{
    bind();

    GLuint textureDepthBufferID;

    glGenTextures(1, &textureDepthBufferID);
    glBindTexture(GL_TEXTURE_2D, textureDepthBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    //filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //border
    float couleurExtremite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, couleurExtremite);

    //wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthBufferID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    depthBufferIDs.emplace_back(textureDepthBufferID);
    std::cout << depthBufferIDs.back() << std::endl;

    unBind();
    return textureDepthBufferID;
}


void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FrameBuffer::unBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBuffer::getID() const
{
    return ID;
}

GLuint FrameBuffer::getTextureColorBuffer(unsigned int index) const
{
    return colorBufferIDs[index];
}
GLuint FrameBuffer::getTextureDepthBuffer(unsigned int index) const
{
    return depthBufferIDs[index];
}

int FrameBuffer::getWidth() const
{
    return width;
}

int FrameBuffer::getHeight() const
{
    return height;
}



FrameBuffer::~FrameBuffer()
{
    unBind();

    glDeleteFramebuffers(1, &ID);
    glDeleteRenderbuffers(1, &renderBufferID);

    colorBufferIDs.clear();
}
