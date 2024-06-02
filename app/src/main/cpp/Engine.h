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

    JNIEnv* attachCurrentThread();
    void detachCurrentThread();

    jstring GetExternalFilesDirJString(JNIEnv* env);

    bool readFile(const std::string& path);
    bool readFile(const char* fileName, std::vector<uint8_t>* buffer);
private:
    inline static Engine* instance = nullptr;
    android_app* mApp;
    EGLDisplay mDisplay;
    EGLint numConfigs;
    EGLint format;
    EGLSurface mSurface;
    EGLContext mContext;
    Shader mShader;
    GLuint mVao, mVbo;
    bool mIsContextInited = false;
};
