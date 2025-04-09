#include "Model3D.hpp"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

bool Model3D::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        return false;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;
    std::unordered_map<Vertex, uint32_t> uniqueVertices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        } else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (prefix == "vt") {
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            texCoord.y = 1.0f - texCoord.y;
            tempTexCoords.push_back(texCoord);
        } else if (prefix == "f") {
            std::string vertexData;
            while (ss >> vertexData) {
                std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                std::istringstream vertexStream(vertexData);
                int vIndex = 0, tIndex = 0, nIndex = 0;
                vertexStream >> vIndex >> tIndex >> nIndex;

                vIndex--; tIndex--; nIndex--;

                if (vIndex < 0 || vIndex >= static_cast<int>(tempVertices.size())) {
                    std::cerr << "Erreur : Indice de vertex invalide: " << (vIndex + 1) << std::endl;
                    continue;
                }
                if (tIndex < 0 || tIndex >= static_cast<int>(tempTexCoords.size())) {
                    std::cerr << "Erreur : Indice de texture invalide: " << (tIndex + 1) << std::endl;
                    continue;
                }
                if (nIndex < 0 || nIndex >= static_cast<int>(tempNormals.size())) {
                    std::cerr << "Erreur : Indice de normale invalide: " << (nIndex + 1) << std::endl;
                    continue;
                }

                Vertex vertex{};
                vertex.position = tempVertices[vIndex];
                vertex.texCoord = tempTexCoords[tIndex];
                vertex.normal = tempNormals[nIndex];

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    if (vertices.empty() || indices.empty()) {
        std::cerr << "Erreur : Modèle vide ou mal formé." << std::endl;
        return false;
    }

    std::cout << "Modèle chargé avec " << vertices.size() << " vertices et " << indices.size() << " indices." << std::endl;
    return true;
}
