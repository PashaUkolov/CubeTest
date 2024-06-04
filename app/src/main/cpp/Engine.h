#pragma once

#include <android_native_app_glue.h>
#include "common.h"
#include "Shader.h"
#include "Mesh.h"

class Engine {
public:
    Engine();
    ~Engine();

    void init();

    static Engine* getInstance();

    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }

    void initDisplay();
    void run(android_app* app);
    void handleCmd(android_app* app, int32_t cmd);

    void moveActionFromJNI(float x, float y);
    void downActionFromJNI(float x, float y);
    void upActionFromJNI(float x, float y);

    JNIEnv* attachCurrentThread();
    void detachCurrentThread();

    jstring GetExternalFilesDirJString(JNIEnv* env);

    bool readFile(const char* fileName, std::vector<uint8_t>* buffer);

private:
    inline static Engine* mInstance = nullptr;
    android_app* mApp;
    EGLDisplay mDisplay;
    EGLint mNumConfigs;
    EGLint mFormat;
    EGLSurface mSurface;
    EGLContext mContext;
    Shader mShader;
    int mHeight = 2340;
    int mWidth = 1080;
    float mCubeRotationX = 0.0f;
    float mCubeRotationY = 0.0f;
    float mTouchPosX = 0.0f;
    float mTouchPosY = 0.0f;
    float mPreviousCubeRotationX = 0.0f;
    float mPreviousCubeRotationY = 0.0f;
    Mesh mCubeMesh;
    bool mIsMoveAction = false;
    bool mIsDownAction = false;
    bool mIsUpAction = false;
    float currX = 0.0f;
    float currY = 0.0f;
};
