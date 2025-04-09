// Renderer3D.hpp
#pragma once
#include <glad/glad.h>  // Toujours inclure glad avant d'inclure d'autres en-têtes OpenGL
#include <glm/glm.hpp>
#include <vector>
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Model3D.hpp"  // Modèle pour le plateau ou autres objets 3D

class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D();

    void init();
    void render(const glm::mat4& projection, GLFWwindow* window, float deltaTime, Camera& camera);

private:
    // Skybox
    GLuint m_shaderProgram;
    Skybox m_skybox;

    // Plateau 3D
    GLuint m_boardShader;
    GLuint m_boardVAO, m_boardVBO, m_boardEBO;
    Model3D m_boardModel;

    GLuint compileShader(const std::string& path, GLenum type);
    GLuint linkProgram(GLuint vs, GLuint fs);
};
