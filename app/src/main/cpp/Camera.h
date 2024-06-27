#pragma once
#include "glm/glm.hpp"
#include "GameObject.h"

struct Camera : public GameObject {
    void draw(Shader shader) override;
    void update() override;
    ~Camera() {}
    glm::mat4 mProjection{1.0f};
    glm::mat4 mView{1.0f};
};