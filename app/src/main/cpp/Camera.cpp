#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine.h"


void Camera::draw(Shader shader) {
    mView = glm::lookAt(mPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
    mProjection = glm::perspective(glm::radians(45.0f), static_cast<float>(Engine::getInstance()->getWidth()) / static_cast<float>(Engine::getInstance()->getHeight()), 0.1f, 1000.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader.mId, "view"), 1, false, glm::value_ptr(mView));
    glUniformMatrix4fv(glGetUniformLocation(shader.mId, "projection"), 1, false, glm::value_ptr(mProjection));
}

void Camera::update() {
}
