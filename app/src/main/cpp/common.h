#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "native_tag_info ", __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "native_tag_error ", __VA_ARGS__);
