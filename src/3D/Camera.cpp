#include "Camera.hpp"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h> // Assure-toi que cette ligne est présente


Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    : m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch),
      m_movementSpeed(2.5f), m_zoom(45.0f), m_radius(10.0f), m_minPitch(-89.0f), m_maxPitch(89.0f)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::keyState(int key, bool isPressed)
{
    m_keys[key] = isPressed; // Met à jour l'état des touches dans le tableau
}

void Camera::processMovement(float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    // Déplacement de la caméra en fonction de l'état des touches
    if (m_keys[GLFW_KEY_W]) {
        m_pitch += m_movementSpeed; // Déplacer vers le haut sur la sphère
    }
    if (m_keys[GLFW_KEY_S]) {
        m_pitch -= m_movementSpeed; // Déplacer vers le bas sur la sphère
    }
    if (m_keys[GLFW_KEY_A]) {
        m_yaw -= m_movementSpeed; // Déplacer à gauche sur la sphère
    }
    if (m_keys[GLFW_KEY_D]) {
        m_yaw += m_movementSpeed; // Déplacer à droite sur la sphère
    }

    // Zoom avec les touches Q et E
    if (m_keys[GLFW_KEY_Q]) {
        m_radius -= m_movementSpeed; // Réduire le rayon pour zoomer vers l'intérieur
    }
    if (m_keys[GLFW_KEY_E]) {
        m_radius += m_movementSpeed; // Augmenter le rayon pour zoomer vers l'extérieur
    }

    // Limiter le zoom (rayon)
    if (m_radius < 5.0f) m_radius = 5.0f;  // Limiter le rayon minimum
    if (m_radius > 50.0f) m_radius = 50.0f;  // Limiter le rayon maximum

    // Limiter le pitch pour ne pas dépasser les bornes
    if (m_pitch > m_maxPitch) m_pitch = m_maxPitch;
    if (m_pitch < m_minPitch) m_pitch = m_minPitch;

    updatePosition(); // Met à jour la position après un changement
}


void Camera::setRadius(float radius)
{
    m_radius = radius;
    updatePosition();
}

void Camera::setPitchLimits(float minPitch, float maxPitch)
{
    m_minPitch = minPitch;
    m_maxPitch = maxPitch;
}

void Camera::updatePosition()
{
    // Calculer la nouvelle position sur la sphère
    m_position.x = m_radius * cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    m_position.y = m_radius * sin(glm::radians(m_pitch));
    m_position.z = m_radius * cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

    // Calculer les vecteurs de la caméra
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // La direction avant de la caméra (regarde vers le centre de la skybox)
    m_front = glm::normalize(m_position - glm::vec3(0.0f));
    
    // Calcul des vecteurs droit et haut
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}
