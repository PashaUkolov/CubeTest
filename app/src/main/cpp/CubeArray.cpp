#include "CubeArray.h"
#include "Engine.h"
#include "common.h"

void CubeArray::init() {
    {
        Cube *cube = new Cube();
        cube->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
        addChild(cube);
    }
    {
        Cube *cube = new Cube();
        cube->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
        addChild(cube);
    }
    {
        Cube *cube = new Cube();
        cube->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
        addChild(cube);
    }
    {
        Cube *cube = new Cube();
        cube->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
        addChild(cube);
    }
    mBoundingBox.min = glm::vec3(-1.5f, -0.5f, -1.4f);
    mBoundingBox.max = glm::vec3(1.5f, 0.5f, 1.5f);
    setPosition({0.0f, 0.0f, 0.0f});
}

void CubeArray::draw(Shader shader) {
}

void CubeArray::update() {
    if (mIsMoved) {
        if (Engine::getInstance()->isDownAction()) {
            mCurrX = Engine::getInstance()->getToucPosX();
            mCurrY = Engine::getInstance()->getToucPosY();
            mPreviousCubeRotationX = mCubeRotationX;
            mPreviousCubeRotationY = mCubeRotationY;
        }

        if (Engine::getInstance()->isMoveAction()) {
            float radius = 1.0f;
            mCubeRotationX = mPreviousCubeRotationX +
                             (mCurrX - Engine::getInstance()->getToucPosX()) /
                             Engine::getInstance()->getWidth();
            mCubeRotationY = mPreviousCubeRotationY +
                             (mCurrY - Engine::getInstance()->getToucPosY()) /
                             Engine::getInstance()->getHeight();
            setRotation(glm::vec3(0.0f, -mCubeRotationX * 2 * PI, 0.0f));
        }

        if (Engine::getInstance()->isUpAction()) {
            mPreviousCubeRotationX = mCubeRotationX;
            mPreviousCubeRotationY = mCubeRotationY;
        }
    }
    mIsMoved = false;
}