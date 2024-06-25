#include "Engine.h"
#include <memory>
#include <fstream>
#include "common.h"
#include <android/asset_manager_jni.h>
#include <android_native_app_glue.h>
#include <jni.h>
#include <pthread.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CubeArray.h"

Engine::Engine() {
}

Engine::~Engine() {
    for(auto& object : mGameObjects) {
        delete object;
    }
}

Engine* Engine::getInstance() {
    if (!mInstance) {
        mInstance = new Engine();
    }

    return mInstance;
}

void Engine::init() {
    for(int i = 0; i < 4; i++) {
        CubeArray* cubeArray = new CubeArray();
        cubeArray->init();
        cubeArray->setPosition({0.0f, (float)i, 0.0f});
        cubeArrays.push_back(cubeArray);
    }

    mShader = Shader("shaders/default");
}

void Engine::initRendering() {
    EGLConfig config = nullptr;
    const EGLint attribs[] = {EGL_RENDERABLE_TYPE,
                              EGL_OPENGL_ES3_BIT,  // Request opengl ES3.0
                              EGL_SURFACE_TYPE,
                              EGL_WINDOW_BIT,
                              EGL_BLUE_SIZE,
                              8,
                              EGL_GREEN_SIZE,
                              8,
                              EGL_RED_SIZE,
                              8,
                              EGL_DEPTH_SIZE,
                              24,
                              EGL_NONE};
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(mDisplay, nullptr, nullptr);
    eglChooseConfig(mDisplay, attribs, nullptr, 0, &mNumConfigs);
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[mNumConfigs]);
    eglChooseConfig(mDisplay, attribs, supportedConfigs.get(), mNumConfigs, &mNumConfigs);
    if (mNumConfigs) {
        LOGI("CONFIG IS SET!");
        config = supportedConfigs[0];
    }
    if (!config) {
        LOGE("failed to init egl config");
        return;
    }

    const EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

    eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &mFormat);
    mSurface = eglCreateWindowSurface(mDisplay, config, mApp->window, nullptr);
    if (mSurface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface, EGL error %d", eglGetError());
        return;
    }

    if (mContext == EGL_NO_CONTEXT) {
        LOGI("NativeEngine: no need to init context (already had one).");
        mContext = eglCreateContext(mDisplay, config, nullptr, contextAttribs);
    }

    if (eglMakeCurrent(mDisplay, mSurface, mSurface, mContext) == EGL_FALSE) {
        LOGE("failed to make context current");
        return;
    }
}

static void handleCmdProxy(struct android_app *app, int32_t cmd) {
    Engine *engine = (Engine *) app->userData;
    engine->handleCmd(app, cmd);
}

bool Engine::isDrawing() {
    return mIsVisible && mIsWindowInited;
}

void Engine::run(android_app* app) {
    mApp = app;
    mApp->userData = this;
    mApp->onAppCmd = handleCmdProxy;

    glViewport(0, 0, mWidth, mHeight);
    while (!mApp->destroyRequested) {
        android_poll_source *pollSource = nullptr;
        auto result = ALooper_pollOnce(0, nullptr, nullptr, (void **) &pollSource);
        if (result == ALOOPER_POLL_ERROR) {
            LOGE("ALooper_pollOnce error!");
            break;
        }

        if (pollSource) {
            pollSource->process(mApp, pollSource);
        }

        mIsMoveAction = false;
        mIsUpAction = false;
        mIsDownAction = false;

        glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(true);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //draw stuff
        if(isDrawing()) {
            if (mIsFirstFrame) {
                mIsFirstFrame = false;
                init();
            }

            glUseProgram(mShader.mId);

            for(auto& cubeArray : cubeArrays) {
                cubeArray->updateChilds();
                cubeArray->update();
                cubeArray->drawChilds(mShader);
                cubeArray->draw(mShader);
            }
        }

        eglSwapBuffers(mDisplay, mSurface);
    }
}

void Engine::handleCmd(android_app *app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            if (app->window != nullptr) {
                initRendering();
                mIsWindowInited = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            mIsWindowInited = false;
        case APP_CMD_START:
            mIsVisible = true;
            break;
        case APP_CMD_STOP:
            mIsVisible = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            break;
        case APP_CMD_LOST_FOCUS:
            break;
        default:
            break;
    }
}

void Engine::moveActionFromJNI(float x, float y) {
    mTouchPosX = x;
    mTouchPosY = y;
    mIsMoveAction = true;
}

void Engine::downActionFromJNI(float x, float y) {
    mTouchPosX = x;
    mTouchPosY = y;
    mIsDownAction = true;
}

void Engine::upActionFromJNI(float x, float y) {
    mTouchPosX = x;
    mTouchPosY = y;
    mIsUpAction = true;
}

static void DetachCurrentThreadDtor(void* p) {
    LOGI("detached current thread");
    if (p != nullptr) {
        ANativeActivity* activity = (ANativeActivity*)p;
        activity->vm->DetachCurrentThread();
    }
}

JNIEnv* Engine::attachCurrentThread() {
    JNIEnv* env;
    if (mApp->activity->vm->GetEnv((void**)&env, JNI_VERSION_1_4) == JNI_OK)
        return env;
    mApp->activity->vm->AttachCurrentThread(&env, NULL);
    pthread_key_t key;
    if (pthread_key_create(&key, DetachCurrentThreadDtor) == 0) {
        pthread_setspecific(key, (void*)mApp->activity);
    }
    return env;
}

void Engine::detachCurrentThread() {
    mApp->activity->vm->DetachCurrentThread();
    return;
}

jstring Engine::GetExternalFilesDirJString(JNIEnv* env) {
    if (mApp->activity == NULL) {
        LOGI("JNIHelper has not been initialized. Call init() to initialize the helper");
        return NULL;
    }

    jstring obj_Path = nullptr;
    jclass clsEnv = env->FindClass("android/app/NativeActivity");
    jmethodID mid = env->GetMethodID(clsEnv, "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;");
    jobject objFile = env->CallObjectMethod(mApp->activity->clazz, mid, NULL);
    if (objFile) {
        jclass cls_File = env->FindClass("java/io/File");
        jmethodID mid_getPath = env->GetMethodID(cls_File, "getPath", "()Ljava/lang/String;");
        obj_Path = (jstring)env->CallObjectMethod(objFile, mid_getPath);
    }
    return obj_Path;
}

bool Engine::readFile(const char* fileName, std::vector<uint8_t>* buffer) {
    if (mApp->activity == NULL) {
        LOGI("JNIHelper has not been initialized.Call init() to initialize the helper");
        return false;
    }

    // Lock mutex
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    // First, try reading from externalFileDir;
    JNIEnv* env = attachCurrentThread();
    jstring strPath = GetExternalFilesDirJString(env);

    std::string s;
    if (strPath) {
        const char* path = env->GetStringUTFChars(strPath, NULL);
        s = std::string(path);
        if (fileName[0] != '/') {
            s.append("/");
        }
        s.append(fileName);
        env->ReleaseStringUTFChars(strPath, path);
        env->DeleteLocalRef(strPath);
    }
    mApp->activity->vm->DetachCurrentThread();
    AAssetManager *assetManager = mApp->activity->assetManager;
    AAsset* assetFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    if (!assetFile) {
        return false;
    }
    uint8_t* data = (uint8_t*) AAsset_getBuffer(assetFile);
    int32_t size = AAsset_getLength(assetFile);
    if (data == NULL) {
        AAsset_close(assetFile);
        LOGI("Failed to load:%s", fileName);
        return false;
    }

    buffer->reserve(size);
    buffer->assign(data, data + size);

    AAsset_close(assetFile);
    return true;
}

void Engine::addGameObject(GameObject* object) {
    mGameObjects.push_back(object);
}
