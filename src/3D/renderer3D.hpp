#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "Skybox.hpp"
#include "Model3D.hpp"


class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D(); // Déclaration du destructeur
    void init();
    void render(const glm::mat4& projection, const glm::mat4& view);

private:
    Skybox m_skybox;
    GLuint m_shaderProgram;

    GLuint compileShader(const std::string& path, GLenum type);
    GLuint linkProgram(GLuint vs, GLuint fs);

    Model3D m_boardModel;
    GLuint m_boardVAO = 0;
    GLuint m_boardVBO = 0;
    GLuint m_boardEBO = 0;
    GLuint m_boardShader = 0;
};
