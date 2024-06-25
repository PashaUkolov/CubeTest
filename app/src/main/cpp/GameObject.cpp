#include "GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

//void TransformComponent::setPosition(glm::vec3 pos) {
//    mPosition = pos;
//}
//
//void TransformComponent::setRotation(glm::vec3 rotation) {
//    mRotation = rotation;
//}
//
//void TransformComponent::setScale(glm::vec3 scale) {
//    mScale = scale;
//}

GameObject::GameObject() {
}

GameObject::~GameObject() {
}

void GameObject::addChild(GameObject *child) {
    child->parent = this;
    mChildren.push_back(child);
}

void GameObject::setPosition(glm::vec3 pos) {
    mPosition = pos;
    //mLocalTransform = glm::translate(mLocalTransform, pos);
    updateChildTransform();
}

void GameObject::setRotation(glm::vec3 rotation) {
    mRotation = rotation;
    //mLocalTransform = glm::rotate(mLocalTransform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    //mLocalTransform = glm::rotate(mLocalTransform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    //mLocalTransform = glm::rotate(mLocalTransform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    updateChildTransform();
}

void GameObject::setScale(glm::vec3 scale) {
    mScale = scale;
    mLocalTransform = glm::scale(mLocalTransform, scale);
    updateChildTransform();
}

glm::mat4 computeTrsMatrix(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale) {
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
    const glm::mat4 rotationMatrix = glm::toMat4(rotation);
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

void GameObject::updateChildTransform() {
    mLocalTransform = computeTrsMatrix(mPosition, mRotation, mScale);
    if (!parent) {
        mGlobalTransform = mLocalTransform;
    }

    for(auto& child : mChildren) {
        child->mGlobalTransform = mGlobalTransform * child->mLocalTransform;
    }
}
