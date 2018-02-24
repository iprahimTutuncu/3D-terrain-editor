#include "Line3D.h"

Line3D::Line3D(glm::vec3 p1, glm::vec3 p2, glm::vec3 color):
    SceneComponent("line3D")
{
    init(p1, p2, color);
}

void Line3D::init(glm::vec3 p1, glm::vec3 p2, glm::vec3 color)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> texture;

    vertices.emplace_back(Vertex(p1, glm::vec2(0,0), glm::vec3(0.0,1.0,0.0)));
    vertices.emplace_back(Vertex(p2, glm::vec2(0,0), glm::vec3(0.0,1.0,0.0)));
    indices.emplace_back(0);
    indices.emplace_back(1);

    this->color = color;
    mesh = std::make_unique<Mesh>(vertices, indices, texture, 0, this->color);
}

void Line3D::draw(Shader &shader)
{
    shader.setVec3("color", this->color);
    mesh->draw(shader, GL_LINES);
    std::cout << "should be drawn" << std::endl;
}

void Line3D::setBeginPoint(glm::vec3 p)
{
    this->pointBegin = p;
    init(this->pointBegin, this->pointEnd, this->color);
}

void Line3D::setEndPoint(glm::vec3 p)
{
    this->pointEnd = p;
    init(this->pointBegin, this->pointEnd, this->color);
}
