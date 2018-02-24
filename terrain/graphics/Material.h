#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

struct MaterialProperties {
    glm::vec3 emission;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};


#endif // MATERIAL_H_INCLUDED
