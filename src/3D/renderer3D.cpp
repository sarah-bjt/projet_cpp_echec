#include "Renderer3D.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <fstream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

Renderer3D::Renderer3D()
    : m_shaderProgram(0), m_boardShader(0), m_boardVAO(0), m_boardVBO(0), m_boardEBO(0) {
    std::cout << "[Renderer3D] Constructeur appelé." << std::endl;
}

Renderer3D::~Renderer3D()
{
    std::cout << "[Renderer3D] Destructeur - Libération des ressources." << std::endl;
    glDeleteVertexArrays(1, &m_boardVAO);
    glDeleteBuffers(1, &m_boardVBO);
    glDeleteBuffers(1, &m_boardEBO);
    glDeleteProgram(m_shaderProgram);
    glDeleteProgram(m_boardShader);
}

glm::vec3 Renderer3D::calculateCenter(const std::vector<Vertex>& vertices)
{
    glm::vec3 center(0.0f);
    for (const Vertex& vertex : vertices) {
        center += vertex.position;
    }
    center /= static_cast<float>(vertices.size());
    return center;
}

void Renderer3D::initializePieces()
{
    std::cout << "[Renderer3D] Initialisation des pièces..." << std::endl;
    std::vector<std::string> whitePieceNames = {
        "rock1", "knight1", "bishop1", "queen", "king", "bishop2", 
        "knight2", "rock2", "pawn1", "pawn2", "pawn3", "pawn4", "pawn5", "pawn6", 
        "pawn7", "pawn8"
    };

    std::vector<std::string> blackPieceNames = {
        "rock1", "knight1", "bishop1", "queen", "king", "bishop2", 
        "knight2", "rock2", "pawn1", "pawn2", "pawn3", "pawn4", "pawn5", "pawn6", 
        "pawn7", "pawn8"
    };

    for (const std::string& name : whitePieceNames) {
        Pion pion;
        pion.modelName = name;
        pion.isWhite = true;
        m_pion.push_back(pion);
        std::cout << "[Renderer3D] Pièce blanche ajoutée : " << name << std::endl;
    }

    for (const std::string& name : blackPieceNames) {
        Pion pion;
        pion.modelName = name;
        pion.isWhite = false;
        m_pion.push_back(pion);
        std::cout << "[Renderer3D] Pièce noire ajoutée : " << name << std::endl;
    }
}

void Renderer3D::drawTexturedModel(GLuint shaderProgram, GLuint textureID, GLuint VAO, const std::vector<GLuint>& indices) {
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLuint textureLoc = glGetUniformLocation(shaderProgram, "texture");
    glUniform1i(textureLoc, 0);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer3D::setupModelBuffers(GLuint& VAO, GLuint& VBO, GLuint& EBO, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2); glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(3); glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Kd));
    glEnableVertexAttribArray(4); glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ks));
    glEnableVertexAttribArray(5); glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ka));
    glEnableVertexAttribArray(6); glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Ns));
    glEnableVertexAttribArray(7); glVertexAttribPointer(7, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, useTexture));

    glBindVertexArray(0);
    std::cout << "[Renderer3D] Buffers créés avec succès." << std::endl;
}

void Renderer3D::init()
{
    std::cout << "[Renderer3D] Initialisation..." << std::endl;

    GLuint vs = compileShader("../../assets/shaders/skybox.vs.glsl", GL_VERTEX_SHADER);
    GLuint fs = compileShader("../../assets/shaders/skybox.fs.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram = linkProgram(vs, fs);

    std::vector<std::string> faces = {
        "../../assets/skybox/left.bmp", "../../assets/skybox/right.bmp",
        "../../assets/skybox/up.bmp", "../../assets/skybox/down.bmp",
        "../../assets/skybox/front.bmp", "../../assets/skybox/back.bmp"
    };
    m_skybox.init(faces);

    std::cout << "[Renderer3D] Chargement du modèle de plateau..." << std::endl;
    if (!m_boardModel.loadFromFile("../../assets/models/board/board.obj", "../../assets/models/board/board.mtl")) {
        std::cerr << "[Renderer3D] ERREUR: Chargement du plateau échoué." << std::endl;
        return;
    }

    GLuint boardVS = compileShader("../../assets/shaders/board.vs.glsl", GL_VERTEX_SHADER);
    GLuint boardFS = compileShader("../../assets/shaders/board.fs.glsl", GL_FRAGMENT_SHADER);
    m_boardShader = linkProgram(boardVS, boardFS);
    setupModelBuffers(m_boardVAO, m_boardVBO, m_boardEBO, m_boardModel.getVertices(), m_boardModel.getIndices());
    
    initializePieces();

    for (auto& pion : m_pion) {
        std::string modelPath = "../../assets/models/White/" + pion.modelName + ".obj";
        std::string mtlPath = "../../assets/models/White/" + pion.modelName + ".mtl";
        std::cout << "[Renderer3D] Chargement du modèle : " << pion.modelName << std::endl;

        if (!pion.model.loadFromFile(modelPath, mtlPath)) {
            std::cerr << "[Renderer3D] ERREUR: Échec chargement pièce : " << pion.modelName << std::endl;
            continue;
        }

        setupModelBuffers(pion.VAO, pion.VBO, pion.EBO, pion.model.getVertices(), pion.model.getIndices());
    }
    

    std::cout << "[Renderer3D] Initialisation terminée." << std::endl;
}

void Renderer3D::render(const glm::mat4& projection, GLFWwindow* window, float deltaTime, Camera& camera)
{
    camera.processMovement(deltaTime);
    glm::mat4 view = camera.getViewMatrix();

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LEQUAL);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    m_skybox.render(m_shaderProgram, projection, skyboxView);
    glDepthFunc(GL_LESS);

    glUseProgram(m_boardShader);

    GLuint projLoc = glGetUniformLocation(m_boardShader, "uProjection");
    GLuint viewLoc = glGetUniformLocation(m_boardShader, "uView");
    GLuint modelLoc = glGetUniformLocation(m_boardShader, "uModel");

    GLuint lightPosLoc = glGetUniformLocation(m_boardShader, "lightPos");
    GLuint viewPosLoc = glGetUniformLocation(m_boardShader, "viewPos");

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::vec3 center = calculateCenter(m_boardModel.getVertices());
    glm::mat4 model = glm::translate(glm::mat4(1.0f), -center);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
    glm::vec3 viewPos = camera.getPosition();
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));

    drawTexturedModel(m_boardShader, m_boardModel.textureID, m_boardVAO, m_boardModel.getIndices());

    for (size_t i = 0; i < m_pion.size(); ++i) {
        const auto& pion = m_pion[i];
        
        glm::vec3 offset(0.0f, 1.0f, 0.0f);
    
        int row = i / 8;
        int col = i % 8;
        offset.x = col;
        offset.z = row;
        
        glUseProgram(m_boardShader);
        
        glm::vec3 pieceCenter = calculateCenter(pion.model.getVertices());
        glm::mat4 pieceModel = glm::translate(glm::mat4(1.0f), -pieceCenter);
        pieceModel = glm::translate(pieceModel, offset);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pieceModel));
        
        drawTexturedModel(m_boardShader, pion.model.textureID, pion.VAO, pion.model.getIndices());
    }
}


GLuint Renderer3D::compileShader(const std::string& path, GLenum type)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Renderer3D] ERREUR: Impossible d'ouvrir le shader : " << path << std::endl;
        return 0;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderCode = buffer.str();
    const char* shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "[Renderer3D] ERREUR: Compilation shader " << path << " : " << infoLog << std::endl;
        return 0;
    }

    std::cout << "[Renderer3D] Shader compilé avec succès : " << path << std::endl;
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
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "[Renderer3D] ERREUR: Linkage du programme : " << infoLog << std::endl;
        return 0;
    }

    std::cout << "[Renderer3D] Programme shader lié avec succès." << std::endl;
    return program;
}
