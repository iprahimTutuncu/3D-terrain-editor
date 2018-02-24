#ifndef LINE3D_H
#define LINE3D_H

#include "Mesh.h"
#include "SceneComponent.h"

#include <memory>

class Line3D: public SceneComponent
{
    public:
        Line3D(glm::vec3 p1, glm::vec3 p2, glm::vec3 color = glm::vec3(1.0,1.0,1.0));
        void init(glm::vec3 p1, glm::vec3 p2, glm::vec3 color = glm::vec3(1.0,1.0,1.0));
        void draw(Shader &shader);
        void setBeginPoint(glm::vec3 p);
        void setEndPoint(glm::vec3 p);
    private:
        std::unique_ptr<Mesh> mesh;
        glm::vec3 color;
        glm::vec3 pointBegin;
        glm::vec3 pointEnd;
};

#endif // LINE3D_H
