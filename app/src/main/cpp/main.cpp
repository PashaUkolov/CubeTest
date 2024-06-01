#include <android_native_app_glue.h>

#include <memory>

#include "Engine.h"

void android_main(android_app* app) {
    Engine engine(app);
    engine.run();
}