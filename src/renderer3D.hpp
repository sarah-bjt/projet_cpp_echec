#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "Skybox.hpp"

class Renderer3D {
public:
    Renderer3D();
    void init();
    void render(const glm::mat4& projection, const glm::mat4& view);

private:
    Skybox m_skybox;
    GLuint m_shaderProgram;

    GLuint compileShader(const std::string& path, GLenum type);
    GLuint linkProgram(GLuint vs, GLuint fs);
};
