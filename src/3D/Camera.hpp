#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    // Constructeur avec un centre autour duquel la caméra tourne
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, const glm::vec3& center);

    glm::mat4 getViewMatrix() const; // Retourne la matrice de vue

    void processMovement(float deltaTime); // Gère le déplacement sans utiliser GLFW_PRESS
    void keyState(int key, bool isPressed); // Enregistre l'état des touches pressées ou relâchées
    void setRadius(float radius); // Définir le rayon (distance de la caméra au centre)
    void setPitchLimits(float minPitch, float maxPitch); // Limiter le pitch
    float getZoom() const { return m_zoom; }; // Retourne le zoom
    float getYaw() const { return m_yaw; } // Retourne l'angle de yaw
    void setYaw(float yaw) { m_yaw = yaw; } // Définir l'angle de yaw
    float getPitch() const { return m_pitch; } // Retourne l'angle de pitch
    void setPitch(float pitch) { m_pitch = pitch; } // Définir l'angle de pitch
    glm::vec3 getPosition() const { return m_position; } // Retourne la position de la caméra

private:
    void updatePosition(); // Met à jour la position de la caméra en fonction du rayon, yaw et pitch
    void updateCameraVectors(); // Met à jour les vecteurs de la caméra (avant, haut, droite)

    glm::vec3 m_position; // Position actuelle de la caméra
    glm::vec3 m_front; // Vecteur directionnel de la caméra
    glm::vec3 m_up; // Vecteur "haut" de la caméra
    glm::vec3 m_right; // Vecteur "droit" de la caméra
    glm::vec3 m_worldUp; // Vecteur "haut" global de la scène
    glm::vec3 m_center; // Centre autour duquel la caméra tourne (le centre du plateau)

    float m_yaw; // Rotation autour de l'axe vertical
    float m_pitch; // Angle d'élévation de la caméra
    float m_movementSpeed; // Vitesse de déplacement de la caméra
    float m_zoom; // Niveau de zoom
    float m_radius; // Rayon de la sphère autour du centre
    float m_minPitch, m_maxPitch; // Limites du pitch (pour éviter les rotations extrêmes)

    bool m_keys[1024]; // Tableau pour stocker l'état des touches
};
