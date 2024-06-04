#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "common.h"
#include "Shader.h"

class Mesh {
public:
    Mesh();
    void initCube();
    void draw(const Shader& shader);
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
private:
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec2> mTextureCoords;
    std::vector<uint32_t> mIndices;
    GLuint mVao, mVbo, mEbo;

    glm::vec3 mPosition;
    glm::vec3 mRotation;
    glm::vec3 mScale;
};