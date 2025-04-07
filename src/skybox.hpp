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

    void init(const std::vector<std::string>& faces);
    void render(GLuint shaderProgram, const glm::mat4& projection, const glm::mat4& view) const;

private:
    GLuint m_skyboxTexture;
    VAO m_vao;
    VBO m_vbo;
    EBO m_ebo;

    void loadCubemap(const std::vector<std::string>& faces);
    void setupSkyboxData();
};