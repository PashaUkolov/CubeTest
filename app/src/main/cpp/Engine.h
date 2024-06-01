#pragma once

#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

class Engine {
public:
    Engine(android_app* app);
    ~Engine();

    void initDisplay();
    void run();
    void handleCmd(android_app* app, int32_t cmd);
private:
    android_app* mApp;
    EGLDisplay mDisplay;
    const EGLint attribs[9] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                              EGL_BLUE_SIZE,    8,
                              EGL_GREEN_SIZE,   8,
                              EGL_RED_SIZE,     8,
                              EGL_NONE};

    EGLint numConfigs;
    EGLint format;
    EGLConfig config = nullptr;
    EGLSurface mSurface;
    EGLContext mContext;
};
