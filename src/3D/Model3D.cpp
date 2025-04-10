#include "Model3D.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <../../import/stb_image.h>

// Fonction pour charger une texture à partir d'un fichier (sans GLEW)
GLuint Model3D::loadTexture(const std::string& texturePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Erreur de chargement de la texture : " << texturePath << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);  // Libère l'image après avoir chargé la texture
    return textureID;
}

// Fonction pour charger le fichier .mtl
std::unordered_map<std::string, Material> Model3D::loadMTL(const std::string& mtlFilename) {
    std::ifstream file(mtlFilename);
    std::unordered_map<std::string, Material> materials;
    if (!file.is_open()) {
        std::cerr << "Erreur lors de l'ouverture du fichier MTL : " << mtlFilename << std::endl;
        return materials;
    }

    std::string line;
    Material currentMaterial;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "newmtl") {
            // Nouveau matériau
            ss >> currentMaterial.name;
        }
        else if (prefix == "map_Kd") {
            // Texture diffuse
            ss >> currentMaterial.textureFile;
            materials[currentMaterial.name] = currentMaterial;
        }
    }

    file.close();
    return materials;
}

bool Model3D::loadFromFile(const std::string& filename, const std::string& mtlFilename) {
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
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "vt") {
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            texCoord.y = 1.0f - texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (prefix == "f") {
            std::string vertexData;
            while (ss >> vertexData) {
                std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                std::istringstream vertexStream(vertexData);
                int vIndex = 0, tIndex = 0, nIndex = 0;
                vertexStream >> vIndex >> tIndex >> nIndex;

                vIndex--; tIndex--; nIndex--; // Ajuste l'indexation à 0

                if (vIndex < 0 || vIndex >= static_cast<int>(tempVertices.size())) continue;
                if (tIndex < 0 || tIndex >= static_cast<int>(tempTexCoords.size())) continue;
                if (nIndex < 0 || nIndex >= static_cast<int>(tempNormals.size())) continue;

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

    // Charger le fichier MTL pour obtenir les matériaux
    std::unordered_map<std::string, Material> materials = loadMTL(mtlFilename);
    if (!materials.empty()) {
        // On suppose que le modèle utilise le matériau "board_material"
        if (materials.find("board_material") != materials.end()) {
            std::string texturePath = materials["board_material"].textureFile;
            textureID = loadTexture(texturePath); // Charger la texture
        }
    }

    return true;
}
