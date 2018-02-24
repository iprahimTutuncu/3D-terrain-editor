#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>
#include <GL/glew.h>
#include "glm/vec3.hpp"
#include "SceneComponent.h"
#include "Mesh.h"
class Grid: public SceneComponent
{
    public:
        Grid();
        void init(int gridSize, float tileSize);
        glm::vec3 getPosition(int i, int j);
        void draw(Shader &shader);
    private:
        GLuint ID;
        std::vector<GLuint> indices;
        std::vector<glm::vec3> positions;
        std::unique_ptr<Mesh> gridMesh;
        int gridSize;
        float tileSize;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;




};

#endif // GRID_H
