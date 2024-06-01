#include "Engine.h"
#include <android/log.h>
#include <memory>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "native_tag_info ", __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "native_tag_error ", __VA_ARGS__);

Engine::Engine(android_app* app) : mApp(app) {
}

Engine::~Engine() {
}

void Engine::initDisplay() {
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(mDisplay, nullptr, nullptr);
    eglChooseConfig(mDisplay, attribs, nullptr, 0, &numConfigs);
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    eglChooseConfig(mDisplay, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    if (numConfigs) {
        LOGI("CONFIG IS SET!");
        config = supportedConfigs[0];
    }
    if(!config) {
        LOGE("failed to init egl config");
        return;
    }

    const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

    eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
    mSurface = eglCreateWindowSurface(mDisplay, config, mApp->window, nullptr);
    if (mSurface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface, EGL error %d", eglGetError());
        return;
    }
    mContext = eglCreateContext(mDisplay, config, nullptr, contextAttribs);
    if (eglMakeCurrent(mDisplay, mSurface, mSurface, mContext) == EGL_FALSE) {
        LOGE("failed to make context current");
        return;
    }
}

static void handleCmdProxy(struct android_app *app, int32_t cmd) {
    Engine *engine = (Engine*)app->userData;
    engine->handleCmd(app, cmd);
}

void Engine::run() {
    mApp->userData = this;
    mApp->onAppCmd = handleCmdProxy;

    while(!mApp->destroyRequested) {
        android_poll_source* pollSource = nullptr;
        auto result = ALooper_pollOnce(0, nullptr, nullptr, (void**)&pollSource);
        if (result == ALOOPER_POLL_ERROR) {
            LOGE("ALooper_pollOnce error!");
            break;
        }

        if(pollSource) {
            pollSource->process(mApp, pollSource);
        }

        if(!mDisplay) {
            LOGE("display is not initialised!");
            return;
        }

        glClearColor(0.9f, 0.0f, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw stuff

        eglSwapBuffers(mDisplay, mSurface);
    }
}

void Engine::handleCmd(android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_SAVE_STATE:

            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != nullptr) {
                initDisplay();
            }
            break;
        case APP_CMD_TERM_WINDOW:

            break;
        case APP_CMD_GAINED_FOCUS:

            break;
        case APP_CMD_LOST_FOCUS:

            break;
        default:
            break;
    }
}
