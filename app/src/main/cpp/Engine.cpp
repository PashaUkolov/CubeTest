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
#include <algorithm>

bool testRayOBBIntersection(
        glm::vec3 ray_origin,        // Ray origin, in world space
        glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
        glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
        glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
        glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
        float &intersection_distance // Output : distance between ray_origin and the intersection with the OBB
);

Engine::Engine() {
}

Engine::~Engine() {
    for (auto &object: mGameObjects) {
        delete object;
    }
}

Engine *Engine::getInstance() {
    if (!mInstance) {
        mInstance = new Engine();
    }

    return mInstance;
}

void Engine::init() {
    for (int i = 0; i < 4; i++) {
        CubeArray *c = new CubeArray();
        c->init();
        c->setPosition({0.0f, -2.0f + (float) i * 1.1f, 0.0f});
        mGameObjects.push_back(c);
        mCubes.push_back(c);
    }

    mCamera = new Camera();
    mCamera->mPosition = glm::vec3(0.0f, 10.0f, -8.0f);
    mGameObjects.push_back(mCamera);

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

void Engine::run(android_app *app) {
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
        if (isDrawing()) {
            if (mIsFirstFrame) {
                mIsFirstFrame = false;
                init();
            }

            glUseProgram(mShader.mId);

            updateCubesControll();

            for (auto &cubeArray: mGameObjects) {
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

static void DetachCurrentThreadDtor(void *p) {
    LOGI("detached current thread");
    if (p != nullptr) {
        ANativeActivity *activity = (ANativeActivity *) p;
        activity->vm->DetachCurrentThread();
    }
}

JNIEnv *Engine::attachCurrentThread() {
    JNIEnv *env;
    if (mApp->activity->vm->GetEnv((void **) &env, JNI_VERSION_1_4) == JNI_OK)
        return env;
    mApp->activity->vm->AttachCurrentThread(&env, NULL);
    pthread_key_t key;
    if (pthread_key_create(&key, DetachCurrentThreadDtor) == 0) {
        pthread_setspecific(key, (void *) mApp->activity);
    }
    return env;
}

void Engine::detachCurrentThread() {
    mApp->activity->vm->DetachCurrentThread();
    return;
}

jstring Engine::GetExternalFilesDirJString(JNIEnv *env) {
    if (mApp->activity == NULL) {
        LOGI("JNIHelper has not been initialized. Call init() to initialize the helper");
        return NULL;
    }

    jstring obj_Path = nullptr;
    jclass clsEnv = env->FindClass("android/app/NativeActivity");
    jmethodID mid = env->GetMethodID(clsEnv, "getExternalFilesDir",
                                     "(Ljava/lang/String;)Ljava/io/File;");
    jobject objFile = env->CallObjectMethod(mApp->activity->clazz, mid, NULL);
    if (objFile) {
        jclass cls_File = env->FindClass("java/io/File");
        jmethodID mid_getPath = env->GetMethodID(cls_File, "getPath", "()Ljava/lang/String;");
        obj_Path = (jstring) env->CallObjectMethod(objFile, mid_getPath);
    }
    return obj_Path;
}

bool Engine::readFile(const char *fileName, std::vector<uint8_t> *buffer) {
    if (mApp->activity == NULL) {
        LOGI("JNIHelper has not been initialized.Call init() to initialize the helper");
        return false;
    }

    // Lock mutex
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    // First, try reading from externalFileDir;
    JNIEnv *env = attachCurrentThread();
    jstring strPath = GetExternalFilesDirJString(env);

    std::string s;
    if (strPath) {
        const char *path = env->GetStringUTFChars(strPath, NULL);
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
    AAsset *assetFile = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
    if (!assetFile) {
        return false;
    }
    uint8_t *data = (uint8_t *) AAsset_getBuffer(assetFile);
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

bool testRayOBBIntersection(
        glm::vec3 ray_origin,        // Ray origin, in world space
        glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
        glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
        glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
        glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
        float &intersection_distance // Output : distance between ray_origin and the intersection with the OBB
) {

    // Intersection method from Real-Time Rendering and Essential Mathematics for Games

    float tMin = 0.0f;
    float tMax = 100000.0f;

    glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

    glm::vec3 delta = OBBposition_worldspace - ray_origin;

    // Test intersection with the 2 planes perpendicular to the OBB's X axis
    {
        glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
        float e = glm::dot(xaxis, delta);
        float f = glm::dot(ray_direction, xaxis);

        if (fabs(f) > 0.001f) { // Standard case

            float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
            float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
            // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

            // We want t1 to represent the nearest intersection,
            // so if it's not the case, invert t1 and t2
            if (t1 > t2) {
                float w = t1;
                t1 = t2;
                t2 = w; // swap t1 and t2
            }

            // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
            if (t2 < tMax)
                tMax = t2;
            // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
            if (t1 > tMin)
                tMin = t1;

            // And here's the trick :
            // If "far" is closer than "near", then there is NO intersection.
            // See the images in the tutorials for the visual explanation.
            if (tMax < tMin)
                return false;

        } else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
            if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Y axis
    // Exactly the same thing than above.
    {
        glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
        float e = glm::dot(yaxis, delta);
        float f = glm::dot(ray_direction, yaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + aabb_min.y) / f;
            float t2 = (e + aabb_max.y) / f;

            if (t1 > t2) {
                float w = t1;
                t1 = t2;
                t2 = w;
            }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        } else {
            if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
                return false;
        }
    }


    // Test intersection with the 2 planes perpendicular to the OBB's Z axis
    // Exactly the same thing than above.
    {
        glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
        float e = glm::dot(zaxis, delta);
        float f = glm::dot(ray_direction, zaxis);

        if (fabs(f) > 0.001f) {

            float t1 = (e + aabb_min.z) / f;
            float t2 = (e + aabb_max.z) / f;

            if (t1 > t2) {
                float w = t1;
                t1 = t2;
                t2 = w;
            }

            if (t2 < tMax)
                tMax = t2;
            if (t1 > tMin)
                tMin = t1;
            if (tMin > tMax)
                return false;

        } else {
            if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
                return false;
        }
    }

    intersection_distance = tMin;
    return true;

}

glm::vec3 Engine::calculateMouseRay() {
    //get normalized device coords
    float x = (2.0f * getToucPosX()) / mWidth - 1.0f;
    float y = 1.0f - (2.0f * getToucPosY()) / mHeight;

    //clip coords
    glm::vec4 clipCoords = glm::vec4(x, y, -1.0f, 1.0f);

    // to eye coords
    glm::vec4 rayEye = glm::inverse(mCamera->mProjection) * clipCoords;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // to world coords
    glm::vec4 rayWorld = glm::inverse(mCamera->mView) * rayEye;
    return glm::normalize(glm::vec3({rayWorld.x, rayWorld.y, rayWorld.z}));
}

void Engine::updateCubesControll() {
    glm::vec3 ray = calculateMouseRay();

    if(mCamera->mPosition.y > 0.0f) {
        std::sort(mCubes.begin(), mCubes.end(), [] (CubeArray* a, CubeArray* b) { return a->mPosition.y > b->mPosition.y; } );
    } else {
        std::sort(mCubes.begin(), mCubes.end(), [] (CubeArray* a, CubeArray* b) { return a->mPosition.y < b->mPosition.y; } );
    }

    if (!mCubes.empty()) {
        for (int i = 0; i < mCubes.size(); ++i) {
            float distance = 0.0f;
            if(testRayOBBIntersection(mCamera->mPosition, ray,mCubes[i]->mBoundingBox.min, mCubes[i]->mBoundingBox.max, mCubes[i]->mGlobalTransform, distance)) {
                // if (distance > 0) {
                mCubes[i]->mIsMoved = true;
                break;
                //break;
                // }
            }
        }
    }
}

void Engine::addGameObject(GameObject *object) {
    mGameObjects.push_back(object);
}
