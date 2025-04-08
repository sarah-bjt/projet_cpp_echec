#include "Skybox.hpp"
#include <iostream>
#include <stb_image.h>

Skybox::Skybox()
    : m_skyboxTexture(0)
{
    // Constructeur : initialisation de la skybox
}

Skybox::~Skybox()
{
    // Destruction : nettoyer les ressources de la skybox
    glDeleteTextures(1, &m_skyboxTexture);
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

    // Initialisation des buffers et du VAO
    m_vao.init();
    m_vbo.init();
    m_ebo.init();

    m_vbo.set_data(skyboxVertices, sizeof(skyboxVertices));
    m_ebo.set_data(skyboxIndices, sizeof(skyboxIndices));

    m_vao.bind();
    m_vbo.bind();
    m_ebo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    m_vao.unbind();
    m_vbo.unbind();
    m_ebo.unbind();
}

void Skybox::loadCubemap(const std::vector<std::string>& faces)
{
    glGenTextures(1, &m_skyboxTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        else
        {
            std::cerr << "Failed to load texture: " << faces[i] << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_TRUE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_TRUE, glm::value_ptr(view));

    // Lier la texture cubemap
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

    // Rendu de la skybox
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36 indices pour un cube
    m_vao.unbind();
}
