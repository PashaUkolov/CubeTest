#pragma once
#include "Mesh.h"
#include "GameObject.h"

class Cube : public GameObject {
public:
    void init();
    void draw(Shader shader) override;
    void update() override;
    virtual ~Cube() {};
private:
    Mesh mCubeMesh;

    float mCubeRotationX = 0.0f;
    float mCubeRotationY = 0.0f;
    float mPreviousCubeRotationX = 0.0f;
    float mPreviousCubeRotationY = 0.0f;
    float mCurrX = 0.0f;
    float mCurrY = 0.0f;
};
