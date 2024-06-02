#pragma once
#include <string>
#include "common.h"

struct Shader {
	Shader() = default;
	explicit Shader(const std::string& path);
	GLuint id;
private:
	void compileShader(const std::string& path);
};