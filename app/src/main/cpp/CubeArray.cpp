#include "CubeArray.h"
#include "Cube.h"
#include "Engine.h"
#include "common.h"

void CubeArray::init() {
    {
        Cube* cube = new Cube();
        cube->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
        addChild(cube);
    }
    {
        Cube* cube = new Cube();
        cube->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
        addChild(cube);
    }
    {
        Cube* cube = new Cube();
        cube->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
        addChild(cube);
    }
    {
        Cube* cube = new Cube();
        cube->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
        addChild(cube);
    }
    setPosition({0.0f, 0.0f, 0.0f});
    //setRotation({90.0f, 0.0f, 0.0f});
}

void CubeArray::draw(Shader shader) {
    //GameObject::draw(shader);
}

void CubeArray::update() {
    //mTimer += 0.1;
    //setRotation({mTimer, 0.0f, 0.0f});
    //LOGE("timer: %f\n", mTimer);
    //GameObject::update();
    //updateChildTransform();
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
        //setPosition(glm::vec3(sinf(mCubeRotationY * 2*PI) * radius, 0.0f, cosf(mCubeRotationY* 2*PI) * radius));
        setRotation(glm::vec3(0.0f, mCubeRotationY * 2*PI, 0.0f));
    }

    if(Engine::getInstance()->isUpAction()) {
        mPreviousCubeRotationX = mCubeRotationX;
        mPreviousCubeRotationY = mCubeRotationY;
    }
}