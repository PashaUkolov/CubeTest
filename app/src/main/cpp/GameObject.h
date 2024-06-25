#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

//class TransformComponent {
//public:
//    void setPosition(glm::vec3 position);
//    void setRotation(glm::vec3 rotation);
//    void setScale(glm::vec3 scale);
//    glm::vec3 getPosition() { return mPosition; };
//    glm::vec3 getRotation() { return mRotation; };
//    glm::vec3 getScale() { return mScale; };
//    glm::mat4 getTransform() { return mTransform; };
//    glm::vec3 mPosition {0.0f};
//    glm::vec3 mRotation;
//    glm::vec3 mScale {1.0f};
//    glm::mat4 mTransform { 1.0f };
//private:
//};
//
//template <typename Child>
//class TransformComponentProxy : public TransformComponent {
//public:
//    virtual void updateChildTransform() {
//    }
//};

class GameObject /*: public TransformComponentProxy<GameObject>*/ {
public:
    GameObject();
    virtual ~GameObject();

    virtual void draw(Shader shader) {};
    virtual void drawChilds(Shader shader) {
        for(auto& child : mChildren) {
            child->draw(shader);
            child->drawChilds(shader);
        }
    }
    virtual void update() {
        LOGE("TRANSFORM UPDATED");
        //TransformComponentProxy::updateChildTransform();

    };

    virtual void updateChilds() {
        //updateChildTransform();
        for(auto& child : mChildren) {
            child->update();
            child->updateChilds();
        }
    }

    void addChild(GameObject* child);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);
    glm::vec3 getPosition() { return mPosition; };
    glm::vec3 getRotation() { return mRotation; };
    glm::vec3 getScale() { return mScale; };

    void updateChildTransform();

    std::vector<GameObject*> mChildren;
    GameObject* parent = nullptr;
    glm::vec3 mPosition{0.0f};
    glm::vec3 mRotation;
    glm::vec3 mScale{1.0f};
    glm::mat4 mLocalTransform{1.0f};
    glm::mat4 mGlobalTransform{1.0f};
};