#include "Skybox.hpp"
#include <iostream>
#include <fstream>
#include <vector>

bool loadBMP(const std::string& filename, int& width, int& height, std::vector<unsigned char>& data) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open BMP file: " << filename << std::endl;
        return false;
    }

    // Lire l'en-tête BMP
    file.seekg(18, std::ios::beg); // Skip header fields
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    // Sauter les champs inutiles (Planes, BitsPerPixel, etc.)
    file.seekg(28, std::ios::beg);

    short bpp;
    file.read(reinterpret_cast<char*>(&bpp), sizeof(bpp)); // Bits per pixel
    if (bpp != 24) {
        std::cerr << "Only 24-bit BMP is supported." << std::endl;
        return false;
    }

    // Lire la palette (si présente) et les pixels
    file.seekg(54, std::ios::beg);  // Sauter jusqu'à la section des pixels
    size_t dataSize = width * height * 3; // 3 bytes par pixel (R, G, B)
    data.resize(dataSize);

    file.read(reinterpret_cast<char*>(data.data()), dataSize);

    // Inverser les lignes (car BMP stocke les pixels de bas en haut)
    int rowSize = width * 3;
    std::vector<unsigned char> rowBuffer(rowSize);
    for (int i = 0; i < height / 2; ++i) {
        std::memcpy(rowBuffer.data(), data.data() + i * rowSize, rowSize);
        std::memcpy(data.data() + i * rowSize, data.data() + (height - 1 - i) * rowSize, rowSize);
        std::memcpy(data.data() + (height - 1 - i) * rowSize, rowBuffer.data(), rowSize);
    }

    return true;
}

// Fonction pour inverser les couleurs si nécessaire
void flipTextureColors(std::vector<unsigned char>& data) {
    for (size_t i = 0; i < data.size(); i += 3) {
        unsigned char temp = data[i];     // Red
        data[i] = data[i + 2];            // Blue
        data[i + 2] = temp;               // Swap Red and Blue
    }
}

Skybox::Skybox()
    : m_skyboxTexture(0), m_floorVAO(0), m_floorVBO(0), m_floorEBO(0)
{
    // Constructeur : initialisation de la skybox et du sol
}

Skybox::~Skybox()
{
    // Destruction : nettoyer les ressources de la skybox et du sol
    glDeleteTextures(1, &m_skyboxTexture);
    glDeleteVertexArrays(1, &m_floorVAO);
    glDeleteBuffers(1, &m_floorVBO);
    glDeleteBuffers(1, &m_floorEBO);
}

void Skybox::setupSkyboxData()
{
    // Vertices du cube pour la skybox
    float skyboxVertices[] = {
        // Positions des coins du cube
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
    };

    unsigned int skyboxIndices[] = {
        0, 1, 2, 0, 2, 3,  // Face avant
        3, 2, 6, 3, 6, 7,  // Face droite
        7, 6, 5, 7, 5, 4,  // Face arrière
        4, 5, 1, 4, 1, 0,  // Face gauche
        5, 6, 2, 5, 2, 1,  // Face bas
        4, 0, 3, 4, 3, 7   // Face haut
    };

    // Initialisation des buffers et du VAO pour la skybox
    m_vao.init();
    m_vbo.init();
    m_ebo.init();

    m_vao.bind(); // Lier AVANT de remplir les buffers
    m_vbo.bind();
    m_vbo.set_data(skyboxVertices, sizeof(skyboxVertices));

    m_ebo.bind();
    m_ebo.set_data(skyboxIndices, sizeof(skyboxIndices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_vao.unbind();

    // Initialisation du sol
    float floorVertices[] = {
        // positions       // normales        // texcoords
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int floorIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &m_floorVAO);
    glGenBuffers(1, &m_floorVBO);
    glGenBuffers(1, &m_floorEBO);

    glBindVertexArray(m_floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Skybox::loadCubemap(const std::vector<std::string>& faces)
{
    glGenTextures(1, &m_skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

    int width, height;
    std::vector<unsigned char> data;
    for (GLuint i = 0; i < faces.size(); i++)
    {
        if (loadBMP(faces[i], width, height, data))
        {
            // Flip colors if needed
            flipTextureColors(data);

            // Assurez-vous que les dimensions sont cohérentes pour toutes les faces de la skybox
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        else
        {
            std::cerr << "Failed to load texture: " << faces[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::init(const std::vector<std::string>& faces)
{
    setupSkyboxData(); // Configure les données de la skybox (vertices, etc.)
    loadCubemap(faces); // Charge les textures de la skybox
}

void Skybox::render(GLuint shaderProgram, const glm::mat4& projection, const glm::mat4& view) const
{
    glUseProgram(shaderProgram);

    // Appliquer les matrices de projection et de vue
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

    // Lier la texture cubemap pour la skybox
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

    // Rendu de la skybox
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36 indices pour un cube
    m_vao.unbind();

    // Rendu du sol
    glBindVertexArray(m_floorVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 indices pour un simple plan
    glBindVertexArray(0);
}
