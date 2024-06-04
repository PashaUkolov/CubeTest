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
    void update();
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
private:
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec2> mTextureCoords;
    std::vector<uint32_t> mIndices;
    GLuint mVao, mVbo, mEbo;

    glm::vec3 mPosition{0.0f};
    glm::vec3 mRotation{0.0f};
    glm::vec3 mScale{1.0f};

    float mCubeRotationX = 0.0f;
    float mCubeRotationY = 0.0f;
    float mPreviousCubeRotationX = 0.0f;
    float mPreviousCubeRotationY = 0.0f;
    float mCurrX = 0.0f;
    float mCurrY = 0.0f;
};