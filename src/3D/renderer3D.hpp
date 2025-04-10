#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Model3D.hpp"
#include <GLFW/glfw3.h>


struct Pion {
    std::string modelName;
    GLuint textureID;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    Model3D model;
    glm::vec3 position = glm::vec3(0.0f);
    bool isWhite;
    bool isWhitePiece() const {
        return isWhite;
    }
};


class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D();

    void init();
    void render(const glm::mat4& projection, GLFWwindow* window, float deltaTime, Camera& camera);
    void initializePieces();
    void setupModelBuffers(GLuint& VAO, GLuint& VBO, GLuint& EBO, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void drawTexturedModel(GLuint shaderProgram, GLuint textureID, GLuint VAO, const std::vector<GLuint>& indices);
    glm::vec3 calculateCenter(const std::vector<Vertex>& vertices);

private:
    // Skybox
    GLuint m_shaderProgram;
    Skybox m_skybox;

    //Pièces
    std::vector<Pion> m_pion;

    // Plateau 3D
    GLuint m_boardShader;
    GLuint m_boardVAO, m_boardVBO, m_boardEBO;
    Model3D m_boardModel;

    GLuint compileShader(const std::string& path, GLenum type);
    GLuint linkProgram(GLuint vs, GLuint fs);
};
