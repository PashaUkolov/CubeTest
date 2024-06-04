#pragma once

#include <jni.h>
#include "common.h"
#include "Engine.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cubetest_MainActivity_handleMoveAction(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    Engine::getInstance()->moveActionFromJNI(x, y);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cubetest_MainActivity_handleDownAction(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    Engine::getInstance()->downActionFromJNI(x, y);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cubetest_MainActivity_handleUpAction(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    Engine::getInstance()->upActionFromJNI(x, y);
}