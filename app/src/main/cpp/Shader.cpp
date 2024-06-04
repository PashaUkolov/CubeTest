#include "Shader.h"
#include <sstream>
#include <fstream>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Engine.h"

Shader::Shader(const std::string& path) {
    compileShader(path);
}

void checkErrors(const unsigned int object, const std::string& type) {
    int success;
    char infoLog[2048];

    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 2048, nullptr, infoLog);
            LOGE("ERROR::SHADER::compile time error Type: %s, %s\n", type.c_str(), infoLog);
        }
    }
    else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 2048, nullptr, infoLog);
            LOGE("ERROR::SHADER::link error Type: %s, %s \n", type.c_str(), infoLog);
        }
    }
}

void Shader::compileShader(const std::string& path) {
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const std::string vsPath = path + std::string(".vs");

    std::vector<uint8_t> vsData;
    Engine::getInstance()->readFile(vsPath.c_str(), &vsData);
    const GLchar* vsSource = (GLchar *)&vsData[0];
    glShaderSource(vertexShader, 1, &vsSource, nullptr);
    glCompileShader(vertexShader);
    checkErrors(vertexShader, "VERTEX");

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const std::string fsPath = path + std::string(".fs");

    std::vector<uint8_t> fsData;
    Engine::getInstance()->readFile(fsPath.c_str(), &fsData);
    const GLchar *fsSource = (GLchar *)&fsData[0];
    glShaderSource(fragmentShader, 1, &fsSource, nullptr);
    glCompileShader(fragmentShader);
    checkErrors(fragmentShader, "FRAGMENT");

    mId = glCreateProgram();
    glAttachShader(mId, vertexShader);
    glAttachShader(mId, fragmentShader);

    glLinkProgram(mId);
    checkErrors(mId, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
