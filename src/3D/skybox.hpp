#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "buffers/VAO.hpp"
#include "buffers/VBO.hpp"
#include "buffers/EBO.hpp"

class Skybox {
public:
    Skybox();
    ~Skybox();

    void init(const std::vector<std::string>& faces); // Initialiser avec les textures de la skybox
    void render(GLuint shaderProgram, const glm::mat4& projection, const glm::mat4& view) const; // Rendu de la skybox

private:
    GLuint m_skyboxTexture; // Identifiant de la texture cubemap
    VAO m_vao;              // VAO pour la skybox
    VBO m_vbo;              // VBO pour la skybox
    EBO m_ebo;              // EBO pour la skybox

    GLuint m_floorVAO;      // VAO pour le sol
    GLuint m_floorVBO;      // VBO pour le sol
    GLuint m_floorEBO;      // EBO pour le sol

    void loadCubemap(const std::vector<std::string>& faces); // Chargement des textures cubemap
    void setupSkyboxData(); // Configuration des données de la skybox (vertices, etc.)
};
