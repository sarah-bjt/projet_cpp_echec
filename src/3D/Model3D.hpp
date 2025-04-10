#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace std {
template<>
struct hash<glm::vec3> {
    size_t operator()(const glm::vec3& v) const {
        size_t h1 = std::hash<float>{}(v.x);
        size_t h2 = std::hash<float>{}(v.y);
        size_t h3 = std::hash<float>{}(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

template<>
struct hash<glm::vec2> {
    size_t operator()(const glm::vec2& v) const {
        size_t h1 = std::hash<float>{}(v.x);
        size_t h2 = std::hash<float>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};
}

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 Kd;
    glm::vec3 Ka;
    glm::vec3 Ks;
    int useTexture;
    float Ns;

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && texCoord == other.texCoord;
    }
};

namespace std {
template<>
struct hash<Vertex> {
    size_t operator()(const Vertex& v) const {
        size_t h1 = std::hash<glm::vec3>{}(v.position);
        size_t h2 = std::hash<glm::vec3>{}(v.normal);
        size_t h3 = std::hash<glm::vec2>{}(v.texCoord);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
}

struct Material {
    std::string name;
    std::string textureFile;
};

class Model3D {
public:
    Model3D() = default;
    ~Model3D() = default;
    GLuint textureID;

    bool loadFromFile(const std::string& filename, const std::string& mtlFilename);

    GLuint loadTexture(const std::string& texturePath);

    std::unordered_map<std::string, Material> loadMTL(const std::string& mtlFilename);

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<uint32_t>& getIndices() const { return indices; }

private:
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<std::string, Material> materials;
};
