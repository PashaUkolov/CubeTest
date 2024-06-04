#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

class TransformComponent {
public:
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setWidth(float newWidth);
    void setHeight(float newHeight);
    void setDepth(float newDepth);
    void setScale(float scale);
    glm::vec3 getPosition() { return mPosition; };
    glm::vec3 getRotation() { return mRotation; };
    float getWidth() { return mWidth; };
    float getHeight() { return mHeight; };
    float getDepth() { return mDepth; };
    float getScale() { return mScale; };
private:
    glm::vec3 mPosition;
    glm::vec3 mRotation;
    float mWidth = 0.0f;
    float mHeight = 0.0f;
    float mDepth = 0.0f;
    float mScale = 1.0f;
};

class GameObject : public TransformComponent {
public:
    GameObject() {};
    virtual ~GameObject();

    virtual void draw(Shader shader) {};
    virtual void update() {};
};