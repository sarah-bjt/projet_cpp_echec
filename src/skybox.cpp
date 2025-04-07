#include "Skybox.hpp"
#include <iostream>
#include <stb_image.h>  // Pour charger les images

Skybox::Skybox()
    : m_skyboxTexture(0) {}

Skybox::~Skybox() {
    glDeleteTextures(1, &m_skyboxTexture);
}

void Skybox::init(const std::vector<std::string>& faces) {
    setupSkyboxData();
    loadCubemap(faces);
}

void Skybox::setupSkyboxData() {
    // Définir les sommets de la skybox (un cube)
    GLfloat skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f
    };

    m_vao.init();
    m_vbo.init();
    m_ebo.init();

    m_vbo.set_data(skyboxVertices, sizeof(skyboxVertices));
    m_vao.bind();
    m_vbo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Pas de données d'index pour la skybox
    m_ebo.bind();
    GLuint indices[] = { 0, 1, 3, 1, 2, 3, 3, 2, 7, 2, 6, 7, 7, 6, 4, 6, 5, 4, 4, 5, 0, 5, 1, 0, 1, 5, 4, 0, 4, 3, 4, 7, 3, 7, 2, 3, 2, 6, 2, 1, 6, 5, 1, 5, 3, 5, 7, 3 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_vao.unbind();
}

void Skybox::loadCubemap(const std::vector<std::string>& faces) {
    glGenTextures(1, &m_skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::render(GLuint shaderProgram, const glm::mat4& projection, const glm::mat4& view) const {
    glUseProgram(shaderProgram);

    // Calculer la matrice de vue sans la translation
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));

    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewNoTranslation));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

    // Désactiver l'écriture dans le tampon de profondeur pendant le rendu de la skybox
    glDepthFunc(GL_LEQUAL);

    m_vao.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_vao.unbind();

    glDepthFunc(GL_LESS);  // Rétablir la fonction de profondeur normale
}
