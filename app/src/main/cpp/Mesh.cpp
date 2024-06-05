#include "Mesh.h"
#include "common.h"
#include "Engine.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh() {
    std::vector<float> vertices = {
            -.5f, -.5f, -.5f, .9f, .9f, .9f,
            -.5f, .5f, .5f, .9f, .9f, .9f,
            -.5f, -.5f, .5f, .9f, .9f, .9f,
            -.5f, .5f, -.5f, .9f, .9f, .9f,

            .5f, -.5f, -.5f, .8f, .8f, .1f,
            .5f, .5f, .5f, .8f, .8f, .1f,
            .5f, -.5f, .5f, .8f, .8f, .1f,
            .5f, .5f, -.5f, .8f, .8f, .1f,

            -.5f, -.5f, -.5f, .9f, .6f, .1f,
            .5f, -.5f, .5f, .9f, .6f, .1f,
            -.5f, -.5f, .5f, .9f, .6f, .1f,
            .5f, -.5f, -.5f, .9f, .6f, .1f,

            -.5f, .5f, -.5f, .8f, .1f, .1f,
            .5f, .5f, .5f, .8f, .1f, .1f,
            -.5f, .5f, .5f, .8f, .1f, .1f,
            .5f, .5f, -.5f, .8f, .1f, .1f,

            -.5f, -.5f, 0.5f, .1f, .1f, .8f,
            .5f, .5f, 0.5f, .1f, .1f, .8f,
            -.5f, .5f, 0.5f, .1f, .1f, .8f,
            .5f, -.5f, 0.5f, .1f, .1f, .8f,

            -.5f, -.5f, -0.5f, .1f, .8f, .1f,
            .5f, .5f, -0.5f, .1f, .8f, .1f,
            -.5f, .5f, -0.5f, .1f, .8f, .1f,
            .5f, -.5f, -0.5f, .1f, .8f, .1f,
    };

    mIndices = {0, 1, 2, 0, 3, 1, 4, 5, 6, 4, 7, 5, 8, 9, 10, 8, 11, 9,
                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(uint32_t), mIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Mesh::draw(const Shader& shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, mPosition);
    model = glm::rotate(model, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 8.0f, -8.0f ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Engine::getInstance()->getWidth()) / static_cast<float>(Engine::getInstance()->getHeight()), 0.1f, 1000.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader.mId, "model"), 1, false, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.mId, "view"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.mId, "projection"), 1, false, glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::setPosition(glm::vec3 position) {
    mPosition = position;
}

void Mesh::setRotation(glm::vec3 rotation) {
    mRotation = rotation;
}