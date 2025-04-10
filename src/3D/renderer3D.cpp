#include "Renderer3D.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <fstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <sstream>

Renderer3D::Renderer3D()
    : m_shaderProgram(0), m_boardShader(0), m_boardVAO(0), m_boardVBO(0), m_boardEBO(0) {}

Renderer3D::~Renderer3D()
{
    glDeleteVertexArrays(1, &m_boardVAO);
    glDeleteBuffers(1, &m_boardVBO);
    glDeleteBuffers(1, &m_boardEBO);
    glDeleteProgram(m_shaderProgram);
    glDeleteProgram(m_boardShader);
}

// Fonction pour calculer le centre du modèle (plateau)
glm::vec3 calculateCenter(const std::vector<Vertex>& vertices)
{
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    for (const Vertex& vertex : vertices) {
        center += vertex.position;
    }
    center /= static_cast<float>(vertices.size());
    return center;
}

void drawTexturedModel(GLuint shaderProgram, GLuint textureID, GLuint VAO, const std::vector<GLuint>& indices) {
    // Activer le VAO de l'objet
    glBindVertexArray(VAO);

    // Activer la première unité de texture (GL_TEXTURE0)
    glActiveTexture(GL_TEXTURE0);

    // Lier la texture à l'unité active
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Récupérer l'emplacement du uniform dans le shader
    GLuint textureLoc = glGetUniformLocation(shaderProgram, "texture");

    // Mettre à jour la valeur du uniform avec l'unité de texture (0 pour GL_TEXTURE0)
    glUniform1i(textureLoc, 0);

    // Dessiner l'objet avec les indices
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

    // Désactiver le VAO
    glBindVertexArray(0);
}

void setupModelBuffers(GLuint& VAO, GLuint& VBO, GLuint& EBO, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    // Générer les objets VAO, VBO et EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Lier le VAO
    glBindVertexArray(VAO);

    // Lier et remplir le VBO avec les données des vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Lier et remplir le EBO avec les indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // Configurer les attributs de vertex

    // Position (attribut 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Normal (attribut 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Texture Coordinate (attribut 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    // Diffuse Color (attribut 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Kd));

    // Specular Color (attribut 4)
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ks));

    // Ambient Color (attribut 5)
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ka));

    // Shininess (attribut 6)
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ns));

    // Texture usage flag (attribut 7)
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, useTexture));

    // Désactiver le VAO
    glBindVertexArray(0);
}



void Renderer3D::init()
{
    // === Skybox shaders ===
    GLuint vs       = compileShader("../../assets/shaders/skybox.vs.glsl", GL_VERTEX_SHADER);
    GLuint fs       = compileShader("../../assets/shaders/skybox.fs.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram = linkProgram(vs, fs);

    std::vector<std::string> faces = {
        "../../assets/skybox/left.bmp",
        "../../assets/skybox/right.bmp",
        "../../assets/skybox/up.bmp",
        "../../assets/skybox/down.bmp",
        "../../assets/skybox/front.bmp",
        "../../assets/skybox/back.bmp"
    };

    m_skybox.init(faces);

    // === Plateau 3D ===
    if (!m_boardModel.loadFromFile("../../assets/models/board/board.obj", "../../assets/models/board/board.mtl"))
    {
        std::cerr << "Erreur : Impossible de charger le modèle du plateau." << std::endl;
        return;
    }

    GLuint boardVS = compileShader("../../assets/shaders/board.vs.glsl", GL_VERTEX_SHADER);
    GLuint boardFS = compileShader("../../assets/shaders/board.fs.glsl", GL_FRAGMENT_SHADER);
    m_boardShader  = linkProgram(boardVS, boardFS);

    setupModelBuffers(m_boardVAO, m_boardVBO, m_boardEBO, m_boardModel.getVertices(), m_boardModel.getIndices());
}

void Renderer3D::render(const glm::mat4& projection, GLFWwindow* window, float deltaTime, Camera& camera)
{
    // Mise à jour de l'état des touches de la caméra
    camera.processMovement(deltaTime);

    // Calcul de la matrice de vue
    glm::mat4 view = camera.getViewMatrix();

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    // Clear buffers (on efface la profondeur et la couleur)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // === Skybox ===
    glDepthFunc(GL_LEQUAL);                            // On permet à la skybox de "passer" au-dessus des autres objets
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Enlever la translation de la vue pour la skybox
    m_skybox.render(m_shaderProgram, projection, skyboxView);
    glDepthFunc(GL_LESS); // On repasse à la profondeur standard pour le reste

    // === Plateau ===
    glUseProgram(m_boardShader);

    GLuint projLoc  = glGetUniformLocation(m_boardShader, "uProjection");
    GLuint viewLoc  = glGetUniformLocation(m_boardShader, "uView");
    GLuint modelLoc = glGetUniformLocation(m_boardShader, "uModel");

    GLuint lightPosLoc = glGetUniformLocation(m_boardShader, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(m_boardShader, "viewPos");

    // Envoi des matrices au shader du plateau
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Calcul du centre du modèle (plateau)
    glm::vec3 center = calculateCenter(m_boardModel.getVertices());

    // Créer la matrice de modèle avec translation pour centrer le modèle
    glm::mat4 model = glm::translate(glm::mat4(1.0f), -center); // Translation pour déplacer le centre à l'origine
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Envoi de la position de la lumière et de la caméra
    glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 0.0f);  // Position de la lumière

    glm::vec3 viewPos = camera.getPosition();  // Position de la caméra
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

    drawTexturedModel(m_boardShader, m_boardModel.textureID, m_boardVAO, m_boardModel.getIndices());
} //

GLuint Renderer3D::compileShader(const std::string& path, GLenum type)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier shader : " << path << std::endl;
        return 0;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderCode = buffer.str();
    const char* shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Vérification des erreurs de compilation
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erreur de compilation du shader (" << path << "):\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

GLuint Renderer3D::linkProgram(GLuint vs, GLuint fs)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // Vérification du lien du programme
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erreur de linkage du programme :\n" << infoLog << std::endl;
        return 0;
    }
    return program;
}
