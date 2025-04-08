#include "Renderer3D.hpp"
#include "Skybox.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

Renderer3D::Renderer3D()
    : m_shaderProgram(0)
{}

void Renderer3D::init()
{
    // Chargement et compilation du shader de skybox
    GLuint vs = compileShader("assets/shaders/skybox.vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = compileShader("assets/shaders/skybox.fs.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram = linkProgram(vs, fs);

    // Textures de la skybox (assure-toi que les chemins sont corrects)
    std::vector<std::string> faces = {
        "assets/skybox/right.png",
        "assets/skybox/left.png",
        "assets/skybox/top.png",
        "assets/skybox/bottom.png",
        "assets/skybox/front.png",
        "assets/skybox/back.png"
    };

    m_skybox.init(faces);
}

void Renderer3D::render(const glm::mat4& projection, const glm::mat4& view)
{
    glDepthFunc(GL_LEQUAL); // Important pour que la skybox s'affiche correctement
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Supprime la translation
    m_skybox.render(m_shaderProgram, projection, skyboxView);
    glDepthFunc(GL_LESS); // Reset pour le reste de la scène
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
