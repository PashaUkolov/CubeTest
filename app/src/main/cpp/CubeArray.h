#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "Cube.h"

struct BoundingBox {
    glm::vec3 min;
    glm::vec3 max;
};

class CubeArray : public GameObject {
public:
    void init();
    void draw(Shader shader) override;
    void update() override;
    ~CubeArray() {}
    bool mIsMoved = false;
    BoundingBox mBoundingBox;
private:
    float mCubeRotationX = 0.0f;
    float mCubeRotationY = 0.0f;
    float mPreviousCubeRotationX = 0.0f;
    float mPreviousCubeRotationY = 0.0f;
    float mCurrX = 0.0f;
    float mCurrY = 0.0f;
    float mTimer = 0.0f;
    std::vector<Cube*> mCubes;

};