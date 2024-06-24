#include "Cube.h"
#include "Engine.h"

void Cube::draw(Shader shader) {
    mCubeMesh.draw(shader, getPosition(), getRotation(), glm::vec3(1.0f));
}

void Cube::update() {
    if(Engine::getInstance()->isDownAction()) {
        mCurrX = Engine::getInstance()->getToucPosX();
        mCurrY = Engine::getInstance()->getToucPosY();
        mPreviousCubeRotationX = mCubeRotationX;
        mPreviousCubeRotationY = mCubeRotationY;
    }

    if(Engine::getInstance()->isMoveAction()) {
        float radius = 1.0f;
        mCubeRotationX = mPreviousCubeRotationX + (mCurrX - Engine::getInstance()->getToucPosX()) / Engine::getInstance()->getWidth();
        mCubeRotationY = mPreviousCubeRotationY + (mCurrY - Engine::getInstance()->getToucPosY()) / Engine::getInstance()->getHeight();
        setPosition(glm::vec3(sinf(mCubeRotationY * 2*PI) * radius, 0.0f, cosf(mCubeRotationY* 2*PI) * radius));
        setRotation(glm::vec3(0.0f, mCubeRotationY * 2*PI, 0.0f));
    }

    if(Engine::getInstance()->isUpAction()) {
        mPreviousCubeRotationX = mCubeRotationX;
        mPreviousCubeRotationY = mCubeRotationY;
    }
}

void Cube::init() {
    LOGE("STUFF IS INITED");
}
