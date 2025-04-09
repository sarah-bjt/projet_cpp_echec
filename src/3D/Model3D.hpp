#ifndef MODEL3D_HPP
#define MODEL3D_HPP

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>


// Spécialisation de std::hash pour glm::vec3
namespace std {
template<>
struct hash<glm::vec3> {
    size_t operator()(const glm::vec3& v) const
    {
        size_t h1 = std::hash<float>{}(v.x);
        size_t h2 = std::hash<float>{}(v.y);
        size_t h3 = std::hash<float>{}(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

// Spécialisation de std::hash pour glm::vec2
template<>
struct hash<glm::vec2> {
    size_t operator()(const glm::vec2& v) const
    {
        size_t h1 = std::hash<float>{}(v.x);
        size_t h2 = std::hash<float>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};
} // namespace std

// Structure Vertex qui utilise glm::vec3 et glm::vec2
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    bool operator==(const Vertex& other) const
    {
        return position == other.position && normal == other.normal && texCoord == other.texCoord;
    }
};

// Spécialisation de std::hash pour Vertex
namespace std {
template<>
struct hash<Vertex> {
    size_t operator()(const Vertex& v) const
    {
        size_t h1 = std::hash<glm::vec3>{}(v.position);
        size_t h2 = std::hash<glm::vec3>{}(v.normal);
        size_t h3 = std::hash<glm::vec2>{}(v.texCoord);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
} // namespace std

class Model3D {
public:
    Model3D()  = default;
    ~Model3D() = default;

    bool loadFromFile(const std::string& filename);

    const std::vector<Vertex>&   getVertices() const { return vertices; }
    const std::vector<uint32_t>& getIndices() const { return indices; }

private:
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
};

#endif // MODEL3D_HPP
