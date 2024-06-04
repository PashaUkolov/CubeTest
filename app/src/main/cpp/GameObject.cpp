#include "GameObject.h"

void TransformComponent::setPosition(glm::vec3 pos) {
    mPosition = pos;
}

void TransformComponent::setRotation(glm::vec3 rotation) {
    mRotation = rotation;
}

void TransformComponent::setWidth(float newWidth) {
    mWidth = newWidth;
}

void TransformComponent::setHeight(float newHeight) {
    mHeight = newHeight;
}

void TransformComponent::setDepth(float newDepth) {
    mDepth = newDepth;
}

void TransformComponent::setScale(float scale) {
    mScale = scale;
}

GameObject::~GameObject() {
}
