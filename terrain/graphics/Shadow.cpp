#include "Shadow.h"
#include "ShaderManager.h"

Shadow::Shadow(float width, float height)
:fbo(width, height)
{
    this->width  = width;
    this->height = height;
}

bool Shadow::init()
{
    fbo.init();
    fbo.genTextureDepthShadowBuffer();

    if(!fbo.isUsable())
        return false;

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f, -0.5f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    setNearPlane(1.0);
    setFarPlane(30.5);

    p_shaderDepth     = RessourceManager::ShaderManager::get("media/shader/depthBased/depthBased.vert", "media/shader/depthBased/depthBased.frag");
    p_shaderShadowMap = RessourceManager::ShaderManager::get("media/shader/shadowMap/shadowMap.vert"  , "media/shader/shadowMap/shadowMap.frag");

    return true;
}

glm::mat4 Shadow::getLightViewProjectionMatrix()
{
    return lightViewProj;
}

void Shadow::update(DirectionalLight& dl, Camera& cam)
{
    setDirectionalLight(dl);
    setCamera(cam);
    setFarPlane(cam.getFarPlane());
    glm::mat4 proj;
    glm::mat4 view;
    glm::vec3 right = glm::normalize(glm::cross(cam.getUp(), lightDir));

    float FOV = cam.getFOV()/2;
    float aspectRatio = cam.getRatio();
    float hnear = 2*tan(glm::radians(FOV)) * cam.getNearPlane();
    float wnear = hnear * aspectRatio;

    float hfar  = 2*tan(glm::radians(FOV)) * getFarPlane();
    float wfar  = hfar * aspectRatio;

    glm::vec3 centerNear = cam.getPosition() + lightDir * cam.getNearPlane();
    glm::vec3 centerFar  = cam.getPosition() + lightDir * getFarPlane();

    glm::vec3 nearTopLeft     = centerNear + cam.getUp() * (hnear/2.0f) - right * (wnear/2.0f);
    glm::vec3 nearTopRight    = centerNear + cam.getUp() * (hnear/2.0f) + right * (wnear/2.0f);
    glm::vec3 nearBottomLeft  = centerNear - cam.getUp() * (hnear/2.0f) - right * (wnear/2.0f);
    glm::vec3 nearBottomRight = centerNear - cam.getUp() * (hnear/2.0f) + right * (wnear/2.0f);

    glm::vec3 farTopLeft     = centerFar + cam.getUp() * (hfar/2.0f) - right * (wfar/2.0f);
    glm::vec3 farTopRight    = centerFar + cam.getUp() * (hfar/2.0f) + right * (wfar/2.0f);
    glm::vec3 farBottomLeft  = centerFar - cam.getUp() * (hfar/2.0f) - right * (wfar/2.0f);
    glm::vec3 farBottomRight = centerFar - cam.getUp() * (hfar/2.0f) + right * (wfar/2.0f);

    view = glm::lookAt(lightDir,
                       glm::vec3(0.0, 0.0, 0.0),
                       glm::vec3(0.0, 1.0, 0.0)
                      );

    glm::vec4 points[8];
    points[0] = view * glm::vec4(nearTopLeft    , 1.0);
    points[1] = view * glm::vec4(nearTopRight   , 1.0);
    points[2] = view * glm::vec4(nearBottomLeft , 1.0);
    points[3] = view * glm::vec4(nearBottomRight, 1.0);
    points[4] = view * glm::vec4(farTopLeft     , 1.0);
    points[5] = view * glm::vec4(farTopRight    , 1.0);
    points[6] = view * glm::vec4(farBottomLeft  , 1.0);
    points[7] = view * glm::vec4(farBottomRight , 1.0);

    float minX = points[0].x;
    float minY = points[0].y;
    float minZ = points[0].z;
    float maxX = points[0].x;
    float maxY = points[0].y;
    float maxZ = points[0].z;

    for(unsigned short i = 1; i < 8; i++){
        if(points[i].x < minX)
            minX = points[i].x;
        else if(points[i].x > maxX)
            maxX = points[i].x;
        if(points[i].y < minY)
            minY = points[i].y;
        else if(points[i].y > maxY)
            maxY = points[i].y;
        if(points[i].z < minZ)
            minZ = points[i].z;
        else if(points[i].z > maxZ)
            maxZ = points[i].z;
    }
    float centerX  = (minX + maxX)/2;
    float centerY  = (minY + maxY)/2;
    float centerZ  = (minZ + maxZ)/2;

    proj = glm::ortho(minX - centerX, maxX + centerX,
                      minY - centerY, maxY + centerY,
                      minZ - centerZ, maxZ + centerZ);


    lightViewProj = proj * view;
}

void Shadow::setDirectionalLight(DirectionalLight& light)
{
    lightDir = light.getLightProperties().direction;
}

void Shadow::setCamera(Camera& camera)
{
    cameraPos = camera.getPosition();
    cameraFront = camera.getFront();
    //setFarPlane(camera.getFarPlane());
}

void Shadow::setNearPlane(float near_plane)
{
    if(near_plane > 0.0)
        this->near_plane = near_plane;
    else
        this->near_plane = 0.0;
}

void Shadow::setFarPlane(float far_plane)
{
    if(far_plane > 0.0)
        this->far_plane = far_plane;
    else
        this->far_plane = 0.0;
}

GLuint Shadow::getDepthTexture() const
{
    return fbo.getTextureDepthBuffer(0);
}

float Shadow::getNearPlane() const
{
    return near_plane;
}


float Shadow::getFarPlane() const
{
    return far_plane;
}

void Shadow::beginDraw()
{
    p_shaderShadowMap->use();
    p_shaderShadowMap->setMat4("viewProj", getLightViewProjectionMatrix());

    glClearColor(0.1f, 0.1f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearDepth(1.0f);

    glViewport(0,0, 2048, 2048);
    fbo.bind();
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glClear(GL_DEPTH_BUFFER_BIT);

}

void Shadow::endDraw()
{
    glDisable(GL_POLYGON_OFFSET_FILL);
    glCullFace(GL_BACK);
    fbo.unBind();
    glViewport(0,0, width, height);

}

void Shadow::drawScreen(GLuint tex) // a effacer
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //init le shader
    p_shaderDepth->use();
    p_shaderDepth->setFloat("near_plane", near_plane);
    p_shaderDepth->setFloat("far_plane", far_plane);
    p_shaderDepth->setInt("screenTexture", 0);

    //bind la texture a affficher
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    //initialiser et bien effacer l'ecran
    glViewport(0,0, 1024, 1024);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //bind le necessaire pour dessiner l'ecran
    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
}

void Shadow::drawShadow(Model& model, glm::mat4 modelTransform)
{
    p_shaderShadowMap->use();
    p_shaderShadowMap->setMat4("model", modelTransform);
    model.Draw(*p_shaderShadowMap);
}

void Shadow::drawShadow(Terrain& terrain, glm::mat4 modelTransform)
{
    p_shaderShadowMap->use();
    p_shaderShadowMap->setMat4("model", modelTransform);
    terrain.draw(*p_shaderShadowMap);
}

Shadow::~Shadow()
{
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);
}
