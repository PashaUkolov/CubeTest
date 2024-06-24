#include "GameObject.h"

void TransformComponent::setPosition(glm::vec3 pos) {
    mPosition = pos;
}

void TransformComponent::setRotation(glm::vec3 rotation) {
    mRotation = rotation;
}

void TransformComponent::setScale(glm::vec3 scale) {
    mScale = scale;
}

GameObject::GameObject() {
}
GameObject::~GameObject() {
}

void GameObject::addChild(GameObject *child) {
    mChildren.push_back(child);
}
