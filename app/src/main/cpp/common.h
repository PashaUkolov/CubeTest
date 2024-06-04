#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/log.h>

#define PI 3.14159265358979323846f
#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "native_tag_info ", __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "native_tag_error ", __VA_ARGS__);
