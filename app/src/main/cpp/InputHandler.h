#pragma once

#include <jni.h>
#include "common.h"
#include "Engine.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cubetest_MainActivity_handleTouchEvent(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    Engine::getInstance()->touchEventFromJNI(x, y);
}