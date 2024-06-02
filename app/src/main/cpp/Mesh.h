#pragma once
#include <string>
#include <vector>

class Mesh {
public:
    Mesh(const std::string& path);
private:
    std::vector<float> mVertices;
    std::vector<float> mNormals;
    std::vector<uint32_t> mIndices;
};