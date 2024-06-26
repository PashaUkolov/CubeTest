#pragma once
#include "common.h"
#include "Shader.h"
#include "GameObject.h"

#include <android_native_app_glue.h>

class Engine {
public:
    Engine();
    ~Engine();

    static Engine* getInstance();

    void init();

    void initRendering();

    bool isDrawing();

    void run(android_app* app);
    void handleCmd(android_app* app, int32_t cmd);

    void moveActionFromJNI(float x, float y);
    void downActionFromJNI(float x, float y);
    void upActionFromJNI(float x, float y);

    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }

    bool isMoveAction() { return mIsMoveAction; }
    bool isDownAction() { return mIsDownAction; }
    bool isUpAction() { return mIsUpAction; }

    float getToucPosX() { return mTouchPosX; }
    float getToucPosY() { return mTouchPosY; }

    void addGameObject(GameObject* object);

    JNIEnv* attachCurrentThread();
    void detachCurrentThread();
    jstring GetExternalFilesDirJString(JNIEnv* env);
    bool readFile(const char* fileName, std::vector<uint8_t>* buffer);

private:
    inline static Engine* mInstance = nullptr;
    android_app* mApp;
    EGLContext mContext = EGL_NO_CONTEXT;
    EGLDisplay mDisplay = EGL_NO_DISPLAY;
    EGLSurface mSurface = EGL_NO_SURFACE;
    EGLConfig mConfig = nullptr;
    EGLint mNumConfigs = 0;
    EGLint mFormat = 0;
    Shader mShader;
    int mHeight = 2340;
    int mWidth = 1080;
    bool mIsMoveAction = false;
    bool mIsDownAction = false;
    bool mIsUpAction = false;
    float mTouchPosX = 0.0f;
    float mTouchPosY = 0.0f;
    std::vector<GameObject*> mGameObjects;
    bool mIsWindowInited = false;
    bool mIsFirstFrame = true;
    bool mIsVisible = false;
};
