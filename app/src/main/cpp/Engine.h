#pragma once

#include <android_native_app_glue.h>
#include "common.h"
//#include <GLES/gl.h>
#include "Shader.h"

class Engine {
public:
    Engine();
    ~Engine();

    void init();

    static Engine* getInstance();

    void initDisplay();
    void run(android_app* app);
    void handleCmd(android_app* app, int32_t cmd);

    void touchEventFromJNI(float x, float y);

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
    GLuint mVao, mVbo;
    bool mIsContextInited = false;
    int mHeight = 2340;
    int mWidth = 1080;
    uint64_t mLastFrameNs;
    float mTime = 0.0f;
    float mCubeRotationX = 0.0f;
    float mCubeRotationY = 0.0f;
};
