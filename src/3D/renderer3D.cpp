#include "Renderer3D.hpp"
#include "Skybox.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

Renderer3D::Renderer3D()
    : m_shaderProgram(0), m_boardShader(0), m_boardVAO(0), m_boardVBO(0), m_boardEBO(0)
{}

Renderer3D::~Renderer3D()
{
    glDeleteVertexArrays(1, &m_boardVAO);
    glDeleteBuffers(1, &m_boardVBO);
    glDeleteBuffers(1, &m_boardEBO);
    glDeleteProgram(m_shaderProgram);
    glDeleteProgram(m_boardShader);
}

void Renderer3D::init()
{
    // === Skybox shaders ===
    GLuint vs = compileShader("C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/shaders/skybox.vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = compileShader("C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/shaders/skybox.fs.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram = linkProgram(vs, fs);

    std::vector<std::string> faces = {
        "C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/skybox/right.bmp",
        "C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/skybox/left.bmp",
        "C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/skybox/up.bmp",
        "C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/skybox/down.bmp",
        "C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/skybox/front.bmp",
        "C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/skybox/back.bmp"
    };

    m_skybox.init(faces);
    std::cout << "Skybox loaded!" << std::endl;

    // === Plateau 3D ===
    if (!m_boardModel.loadFromFile("C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/models/board/board.obj")) {
        std::cerr << "Erreur : Impossible de charger le modèle du plateau." << std::endl;
        return;
    }

    GLuint boardVS = compileShader("C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/shaders/board.vs.glsl", GL_VERTEX_SHADER);
    GLuint boardFS = compileShader("C:/Users/janag/Documents/Cours/ANNEE 2/c++/Echec/projet_cpp_echec/assets/shaders/board.fs.glsl", GL_FRAGMENT_SHADER);
    m_boardShader = linkProgram(boardVS, boardFS);

    glGenVertexArrays(1, &m_boardVAO);
    glGenBuffers(1, &m_boardVBO);
    glGenBuffers(1, &m_boardEBO);

    glBindVertexArray(m_boardVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_boardVBO);
    glBufferData(GL_ARRAY_BUFFER, m_boardModel.getVertices().size() * sizeof(Vertex), m_boardModel.getVertices().data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_boardEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_boardModel.getIndices().size() * sizeof(uint32_t), m_boardModel.getIndices().data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void Renderer3D::render(const glm::mat4& projection, const glm::mat4& view)
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // === Skybox ===
    glDepthFunc(GL_LEQUAL);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    m_skybox.render(m_shaderProgram, projection, skyboxView);
    glDepthFunc(GL_LESS);

    // === Plateau ===
    glUseProgram(m_boardShader);

    GLuint projLoc = glGetUniformLocation(m_boardShader, "uProjection");
    GLuint viewLoc = glGetUniformLocation(m_boardShader, "uView");
    GLuint modelLoc = glGetUniformLocation(m_boardShader, "uModel");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(m_boardVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_boardModel.getIndices().size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

GLuint Renderer3D::compileShader(const std::string& path, GLenum type)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return 0;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string sourceStr = ss.str();
    const char* source = sourceStr.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compilation failed (" << path << "):\n" << log << std::endl;
    }

    return shader;
}

GLuint Renderer3D::linkProgram(GLuint vs, GLuint fs)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Program linking failed:\n" << log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
