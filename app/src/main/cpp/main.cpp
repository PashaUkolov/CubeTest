#include <android_native_app_glue.h>
#include "Engine.h"

void android_main(android_app* app) {
    Engine* engine = Engine::getInstance();
    engine->run(app);
}